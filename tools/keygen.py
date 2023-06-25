import os
import sys
import time
from multiprocessing import Process, Queue, Value
import random
from math import gcd
COMMAND_CLEAR = -1
COMMAND_STOP_ALL_PROCESS = -2

#p,q must be prime
#make sure p*q is not bigger than target dataatype
def generate_keypair(p, q):
    n = p * q
    phi = (p - 1) * (q - 1)
    e = random.randrange(1, phi)
    g = gcd(e, phi)
    while g != 1:
        e = random.randrange(1, phi)
        g = gcd(e, phi)
    d = pow(e, -1, phi)
    return (n, e), (n, d)

def encrypt(public_key, message):
    n, e = public_key
    cipher = pow(message, e, n)
    return cipher

def decrypt(private_key, cipher):
    n, d = private_key
    message = pow(cipher, d, n)
    return message

"""
it's the same as check_prime, but with specified factor range to check for multicore threading
check if n has factor of number between start~end
@param start: >=2
@param queue: 
    will be appended a [False] if n has factor
    will be appended a [True] if n has no factor
"""
def find_factor(thread_id, n,start,end,queue,command):
    # ~ print(f"dimulai {n},{start},{end},{queue}")
    starttime = time.time()
    #only check prime_number as factor
    for i in range(start,end):
        #checking all number between start~end is fine, but
        #only check prime_number as factor is more efficient;
        #prime number is always 6k-1 or 6k+1, even though not all 6k-1 or 6k+1 is prime
        if (i-1)%6==0 or (i+1)%6==0 or i==2 or i==3:
            pass # (i) is probably prime number, check if (i) is factor of (n)
        else :
            #   (i) is not (6k-1 or 6k+1).
            # it means 2 or 3 is factor of (i)
            # reaching here means (2) or (3) is not factor of (n)
            # then (i) which is multiply of (2) or (3) must not be factor of (n)
            continue 

        # ~ if i%100==0:print(f"comparing {i}")
        if command.value==thread_id:
            elapsed = time.time()
            print(f"from thread #{thread_id}")
            print(f"progress: {i} percent: {100*(i-start)/(end-start)}%")
            print(f"elapsed: {elapsed-starttime}")
            print(f"speed: {(i-start)/(elapsed-starttime)} item/second")
            
            with command.get_lock():
                command.value = COMMAND_CLEAR #clear
        if n%i==0 or command.value==COMMAND_STOP_ALL_PROCESS:
            with command.get_lock():
                command.value = COMMAND_STOP_ALL_PROCESS
            queue.put(False)
            break
        
        if i==(end-1):
            #if all potential factor is checked and none found, and taking too long,
            #it's intersting to know the speed
            elapsed = time.time()
            if (elapsed-starttime)>3:
                print(f"elapsed: {elapsed-starttime}")
                print(f"speed: {(i-start)/(elapsed-starttime)} item/second")

    queue.put(True)

def check_prime(v,numthreads=4,unattended=True):
    if (v-1)%6==0 or (v+1)%6==0:
        pass
    else:
        return False
    if v<2:
        return False
    q = Queue()
    command = Value('i',COMMAND_CLEAR)
    threads = []
    part_start = 2
    part_end = 2
    try:
        search_limit = round(v**0.5)+1 #checking prime only need to find factor bellow squareroot of (v)
    except TypeError:
        print(f"error here with v = {v}")
        exit(1)
    for t in range(numthreads): #separate search_limit into #numthreads
        part_start = part_end
        part_end = search_limit*(t+1)//numthreads
        t = Process(target=find_factor, args=(t, v, part_start, part_end, q, command, ), daemon=True)
        t.start()
        threads.append(t)
    
    line = ""
    # ~ print (f"sampai sini {v} {unattended}")
    while not(unattended):#dont do this if unattended=True
        if line=="exit":
            print("force exit")
            break
        if line.isnumeric():
            thread_id = int(line)
            with command.get_lock():
                command.value = thread_id
        alldone = True
        for thread in threads:
            if thread.is_alive():
                alldone = False
                break
        if alldone:
            # ~ print("process done.")
            break
        print("enter number of thread_id to check progress, or x to exit")
        line = input()
        

    for thread in threads:
        thread.join()
    
    is_prime = True #assume it is prime
    for t in threads: #expect count if queue equals len(threads)
        try:
            is_prime = q.get(block=True,timeout=0.5)
        except:pass
        if not is_prime:#it is not prime, no need to continue process
            return False
            
    
    return True #if it reached here, it is prime

def get_closest_prime(n,numthreads=4,unattended=True):
    maks = n
    
    for x in range(maks//6,maks//6-500,-1):
        prime = check_prime(6*x-1,numthreads,unattended)
        if prime:
            return 6*x-1
        prime = check_prime(6*x+1,numthreads,unattended)
        if prime:
            return 6*x+1
    return None

def random16():    
    bytedata = os.urandom(2)
    return bytedata[0]
    return (bytedata[0]<<8)+bytedata[1]

def make_many_key(howmuch):
    for i in range(howmuch):
        p,q = None,None
        while p==None or q==None:
            p = get_closest_prime(random16())
            q = get_closest_prime(random16())
        key = (generate_keypair(p,q))
        print(key[0][0],key[0][1],key[1][1],key[0][0]<0x7fffffff,sep=", ")

if __name__=="__main__":
    make_many_key(100)
