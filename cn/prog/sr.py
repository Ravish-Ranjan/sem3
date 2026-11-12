
import random
import time

TOTAL_PACKETS = 10
WINDOW_SIZE = 7
TOTAL_FAILURE_RATE = 0.7
TIMEOUT = 0.05


class Channel:
    def __init__(self):
        self.total_transmissions = 0
        self.failed_transmission = 0

    def transmit(self):
        self.total_transmissions += 1
        success = random.random() >= TOTAL_FAILURE_RATE

        if not success:
            self.failed_transmission += 1

        return success


class Reciever:
    def __init__(self, window_size):
        self.expected = 0
        self.window_size = window_size
        self.buffer = set()

    def recieve(self, packet: int):
        if self.expected <= packet < self.expected + self.window_size:
            self.buffer.add(packet)
            print(f" ok [Reciever] pkt{packet}")

            while self.expected in self.buffer:
                self.buffer.remove(self.expected)
                self.expected += 1

            return True
        elif packet < self.expected:
            print(f" duplicate [Reciever] pkt{packet}")
            return True

        print(f" not ok [Reciever] pkt{packet}")
        return False


class Sender:
    def __init__(self, window_size):
        self.window_size = window_size
        self.total_transmission_done = 0
        self.total_failiures = 0
        self.total_successful = 0

    def start_tranmission(self, to: Reciever,total_to_transmit: int, via: Channel):
        base = 0
        cursor = 0
        timers = {}
        outstanding = set()
        acknowledged = set()

        while base < total_to_transmit:
            while cursor < min(base + self.window_size, total_to_transmit):
                self.total_transmission_done += 1
                print(f"[Sender] pkt{cursor} -----", end="")
                success = via.transmit()

                if success:
                    ack = to.recieve(cursor)

                    if ack:
                        acknowledged.add(cursor)
                        self.total_successful += 1
                    else:
                        outstanding.add(cursor)
                        timers[cursor] = time.monotonic()
                else:
                    print(" packet lost")
                    outstanding.add(cursor)
                    timers[cursor] = time.monotonic()

                cursor += 1

            while base in acknowledged:
                acknowledged.remove(base)
                outstanding.discard(base)
                timers.pop(base, None)
                base += 1

            current_time = time.monotonic()

            for packet in list(outstanding):
                if current_time - timers[packet] >= TIMEOUT:
                    print(f"[Sender] TIMEOUT pkt{packet}")
                    print(f"[Sender] Retransmitting pkt{packet}")
                    self.total_transmission_done += 1
                    success = via.transmit()

                    if success:
                        ack = to.recieve(packet)

                        if ack:
                            acknowledged.add(packet)
                            outstanding.remove(packet)
                            timers.pop(packet, None)
                            self.total_successful += 1
                        else:
                            timers[packet] = time.monotonic()

                    else:
                        print(f" packet lost")
                        timers[packet] = time.monotonic()
                        self.total_failiures += 1

            while base in acknowledged:
                acknowledged.remove(base)
                outstanding.discard(base)
                timers.pop(base, None)
                base += 1

            if outstanding:
                time.sleep(0.001)

            print("-" * 30)

    def get_results(self):
        print(f"""
Total transmission : {self.total_transmission_done}
Successful : {self.total_successful}
Failure : {self.total_failiures}
Re-transmissions : {self.total_transmission_done - self.total_successful}
Failure rate : {"0.00" if self.total_transmission_done == 0 else round((self.total_failiures * 100) / self.total_transmission_done, 2)}%
""")


ch = Channel()
rs = Reciever(WINDOW_SIZE)
sd = Sender(WINDOW_SIZE)

sd.start_tranmission(rs, TOTAL_PACKETS, ch)
sd.get_results()
