#!/opt/miniconda/miniconda3/bin/python3
#sending serial for otp
import time
import sys
import datetime
import pyotp
from serialsender import kirim

def timecode(for_time: datetime.datetime, interval=30) -> int:
    """
    Accepts either a timezone naive (`for_time.tzinfo is None`) or
    a timezone aware datetime as argument and returns the
    corresponding counter value (timecode).
    """
    if for_time.tzinfo:
        return int(calendar.timegm(for_time.utctimetuple()) / interval)
    else:
        return int(time.mktime(for_time.timetuple()) / interval)



def otp(securedataname_for_otpkey):

    kirim([
        securedataname_for_otpkey
        ,10
        ,str(int(
            # ~ (datetime.datetime.now()-datetime.datetime(1970,1,1)).total_seconds()
            # ~ (datetime.datetime.utcnow()-datetime.datetime(1970,1,1)).total_seconds()
            # ~ time.time()
            timecode(datetime.datetime.now())
         ))+";" # append non digit charracter ";"as string so 
                # serialsender.kirim treat it as string and send each
                # string charracter as byte data and 
                # secureelemakin_thumb.ino will parseInt and stop
                # parsing digit upon reaching charracter ";"
        ,10
    ])
if __name__=="__main__":
    securedata_name = sys.argv[1]
    otp(securedata_name)
