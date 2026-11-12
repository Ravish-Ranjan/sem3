import random
import threading
import time

TOTAL_PACKETS = 100
WINDOW_SIZE = 7
TOTAL_FAILURE_RATE = 0.1
TIMEOUT = 0.05

class Channel:
    def __init__(self):
        self.total_transmissions = 0
        self.failed_transmission = 0

    def transmit(self):
        self.total_transmissions += 1
        success = random.random() >= TOTAL_FAILURE_RATE

        if not(success):
            self.failed_transmission += 1

        return success

class Reciever:
    def __init__(self):
        self.expected = 0

    def recieve(self,packet:int):
        if (packet == self.expected):
            print(f" ok [Reciever]",)
            self.expected += 1
        else :
            print(f" not ok [Reciever]")

        return self.expected

class Sender:
    def __init__(self,window_size):
        self.window_size = window_size
        self.total_transmission_done = 0
        self.total_failiures = 0
        self.total_successful = 0

    def start_tranmission(self,to:Reciever,total_to_transmit:int,via:Channel):
        self.total_to_transmit = total_to_transmit
        base = 0
        cursor = 0
        timer_start = None

        while base < self.total_to_transmit:
            while cursor < min(base + self.window_size,self.total_to_transmit):
                self.total_transmission_done += 1 
                print(f"[Sender] pkt{cursor} -----",end="")
                success = via.transmit()

                if (success):
                    ack = to.recieve(cursor)

                    if (ack > base):
                        self.total_successful += 1
                        base = ack

                        if base < (cursor + 1):
                            timer_start = time.monotonic()
                        else :
                            timer_start = None
                    else :
                        self.total_failiures += 1
                else :
                    self.total_failiures += 1
                    print(f" packet lost")

                cursor += 1

            if base < cursor:
                if timer_start is None:
                    timer_start = time.monotonic()

                time_spent = time.monotonic() - timer_start
                remaining = TIMEOUT - time_spent

                if (remaining > 0):
                    print(f"[Sender] waiting for ACKs for {remaining}")
                    time.sleep(remaining)

                print("TIMEOUT")
                print(f"Retransmitting pkts from {base} to {cursor-1}")

                cursor = base
                timer_start = None
            print("-"*30)

    def get_results(self):
        print(f"""
Total tranmission : {self.total_transmission_done}
Successful : {self.total_successful}
Failiure : {self.total_failiures}
Re-tranmissions : {self.total_transmission_done - self.total_successful}
Failure rate : {"0.00" if self.total_transmission_done == 0 else round((self.total_failiures*100) / self.total_transmission_done,2)}%
""")


ch = Channel()
rs = Reciever()
sd = Sender(WINDOW_SIZE)

sd.start_tranmission(rs,TOTAL_PACKETS,ch)
sd.get_results()