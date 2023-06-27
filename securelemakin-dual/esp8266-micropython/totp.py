import base64
import hashlib
import hmac

import ntptime
import time
import gc

class OTP(object):
    """
    Base class for OTP handlers.
    """

    def __init__(
        self,
        s: str,
        digits: int = 6,
    ) -> None:
        self.digits = digits
        if digits > 10:
            raise ValueError("digits must be no greater than 10")
        self.digest = hashlib.sha1
        self.secret = s

    def generate_otp(self, input_: int) -> str:
        """
        :param input: the HMAC counter value to use as the OTP input.
            Usually either the counter, or the computed integer based on the Unix timestamp
        """
        if input_ < 0:
            raise ValueError("input must be positive integer")
        hasher = hmac.new(self.byte_secret(), self.int_to_bytestring(input_), self.digest)
        hmac_hash = bytearray(hasher.digest())
        offset = hmac_hash[-1] & 0xF
        code = (
            (hmac_hash[offset] & 0x7F) << 24
            | (hmac_hash[offset + 1] & 0xFF) << 16
            | (hmac_hash[offset + 2] & 0xFF) << 8
            | (hmac_hash[offset + 3] & 0xFF)
        )
        str_code = str(10_000_000_000 + (code % 10**self.digits))
        return str_code[-self.digits :]

    def byte_secret(self) -> bytes:
        secret = self.secret
        missing_padding = len(secret) % 8
        if missing_padding != 0:
            secret += "=" * (8 - missing_padding)
        return base64.b32decode(secret, casefold=True)

    @staticmethod
    def int_to_bytestring(i: int, padding: int = 8) -> bytes:
        """
        Turns an integer to the OATH specified
        bytestring, which is fed to the HMAC
        along with the secret
        """
        result = bytearray()
        while i != 0:
            result.append(i & 0xFF)
            i >>= 8
        # It's necessary to convert the final result from bytearray to bytes
        # because the hmac functions in python 2.6 and 3.3 don't work with
        # bytearray
        
        #right justify / right pad
        # ~ if jenis:
            # ~ return bytes(bytearray(reversed(result)).rjust(padding, b"\0"))
        
        #but in micropython rjust is not exist, manual rjust:
        return bytes(
            bytearray([0]*(padding))+
            bytearray(reversed(result))
        )[-padding:]#maxlength is padding
    
# micropython use epochh since jan 2000, but TOTP compatible with google authenticator use unix epoch since 1970

# esp8266 precompiled firmware settime() doesnt accept parameter,so 
# time.localtime() is always at UTC, adjust manually is possible but 
# google authenticator use UTC too

def epoch_2000_to_1970(s):
    return s+946_684_800
    

def now(secretcode):
    aku = OTP(secretcode)
    ntptime.settime()
    hasil =  aku.generate_otp(
        int(
            epoch_2000_to_1970(time.mktime(time.localtime()))
            //30 #TOTP timestep
        )
    )
    aku = None
    gc.collect()
    return hasil
    
# ~ print(aku.generate_otp(int(time.mktime(datetime.datetime.now().timetuple())/30)))

