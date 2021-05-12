class BitstreamReader():
    def __init__(self, bytes, bytesize=8):
        self.bytes = bytes
        self.bytesize = bytesize
        self.pos = 0
        self.bits = 0
        self.value = 0

    def remaining(self):
        if self.pos < len(self.bytes):
            return True
        if self.bits > 0:
            return True
        return False

    def read(self, count):
        while self.bits < count:
            if self.pos >= len(self.bytes):
                break
            self.value |= (self.bytes[self.pos] & ((1 << self.bytesize) - 1)) << self.bits
            self.bits += self.bytesize
            self.pos += 1

        ret = self.value & ((1 << count) - 1)
        self.value >>= count
        self.bits -= count
        return ret

class BitstreamWriter():
    def __init__(self, bytesize=8):
        self.bytes = []
        self.bytesize = bytesize
        self.bits = 0
        self.value = 0

    def finish(self):
        if self.bits > 0:
            self.bytes.append(self.value & ((1 << self.bytesize) - 1))
        return self.bytes

    def write(self, value, bits):
        self.value |= (value & ((1 << bits) - 1)) << self.bits
        self.bits += bits
        while self.bits >= self.bytesize:
            self.bytes.append(self.value & ((1 << self.bytesize) - 1))
            self.value >>= self.bytesize
            self.bits -= self.bytesize
