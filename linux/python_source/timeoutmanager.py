import threading
import time
#using threading instead of multiprocessing, so callback_function can be shared
class TimeoutManager(threading.Thread):
  def __init__(self, callback_function, timeout_seconds):
    super().__init__()
    self.daemon = True
    self.cb = callback_function
    self.timeout_ms = int(timeout_seconds*1000)
    self.canceled = False

  def run(self):
    c = 0
    while not self.canceled:
      time.sleep(0.1)
      c += 100
      if c>=self.timeout_ms:
        break
    if not self.canceled:
      self.cb()
  
  def cancel(self):
    self.canceled = True

def setTimeout(callback_function, timeout_seconds):
  tobj = TimeoutManager(callback_function, timeout_seconds)
  tobj.start()
  return tobj

def clearTimeout(timeout_obj):
  try:
    timeout_obj.cancel()
  except:pass


def wait_ctrl_c():
  try:
    while True:pass
  except KeyboardInterrupt:
    print('bye2')
