import sys
import time
from multiprocessing import Process, Queue, Value

def find_factor(thread_id, n,start,end,queue,command):
    print(f"dimulai {n},{start},{end},{queue}")
    starttime = time.time()
    for i in range(start,end):
        # ~ if i%100==0:print(f"comparing {i}")
        if command.value==thread_id:
            elapsed = time.time()
            print(f"from thread #{thread_id}")
            print(f"progress: {i}")
            print(f"elapsed: {elapsed-starttime}")
            print(f"speed: {(i-start)/(elapsed-starttime)} item/second")
            
            with command.get_lock():
                command.value = (-1) #clear
        if n%i==0:
            queue.put(False)
            # ~ print("dudu prime")
            break
    # ~ print("koyone prime")
    queue.put(True)

def check_prime(v,numthreads=4):
    if (v-1)%6==0 or (v+1)%6==0:
        pass
    else:
        return False
    q = Queue()
    command = Value('i',-1)
    threads = []
    part_start = 2
    part_end = 2
    for t in range(numthreads):
        part_start = part_end
        part_end = v*(t+1)//numthreads
        print(f"part: {part_start}/{part_end}")
        t = Process(target=find_factor, args=(t, v, part_start, part_end, q, command, ), daemon=True)
        t.start()
        threads.append(t)
        # ~ t.join()
    print("checking now")
    is_prime = False
    
    # ~ print("enter number of thread_id to check progress, or x to exit")
    # ~ line = input()
    while False:
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
            print("process done.")
            break
        line = input()
    for t in range(numthreads):
        is_prime = q.get(block=True,timeout=0.5)
        print(f"Is {v} prime?: {is_prime}")
        if not is_prime:
            break
            return False
    return is_prime


if __name__=="__main__":
    # ~ print(f"checkprime: {check_prime(65521)}")
    maks = 55521
    if len(sys.argv)>1:
        maks = int(sys.argv[1])
    for x in range(maks//6,maks//6-500,-1):
        prime = check_prime(6*x-1)
        if prime:
            # ~ print(f"found prime: {prime}")
            break
        prime = check_prime(6*x+1)
        if prime:
            # ~ print(f"found prime: {prime}")
            break
        
    # ~ check_prime(4294967291)
    # ~ check_prime(4294967290)
