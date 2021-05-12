# Very simple PNG encoder with *no* compression
# Suitable to generate easily-reproducible PNGs from for example game ROMs

from binascii import crc32

COLOR_GRAY = 0  # Each pixel is a grayscale sample.
COLOR_RGB = 2  # Each pixel is an R,G,B triple.
COLOR_PLTE = 3  # Each pixel is a palette index; a PLTE chunk must appear.
COLOR_GRAYA = 4  # Each pixel is a grayscale sample, followed by an alpha sample.
COLOR_RGBA = 6  # Each pixel is an R,G,B triple, followed by an alpha sample.

class PNGWriter():
    def __init__(self, filename, width, height, depth=8, color=COLOR_RGB):
        self.file = open(filename, "wb")

        # http://png-mng.sourceforge.net/pub/png/spec/1.2/png-1.2-pdg.html#C.IHDR
        self.file.write(b"\211PNG\r\n\032\n")
        chunk = b"IHDR"
        chunk += width.to_bytes(4, "big") + height.to_bytes(4, "big")
        chunk += bytes((depth, color, 0, 0, 0))
        self.write_chunk(chunk)
        
        self.width = width
        self.height = height
        self.depth = depth
        self.color = color

    def write_chunk(self, chunk):
        self.file.write((len(chunk) - 4).to_bytes(4, "big"))
        self.file.write(chunk)
        self.file.write(crc32(chunk).to_bytes(4, "big"))

    def write_palette(self, palette):
        chunk = bytearray(b"PLTE")
        for x in palette:
            chunk += bytes(x[:3])
        self.write_chunk(chunk)

    def write_data(self, data):
        # Pack the image data into a byte array (no filtering)
        packed = bytearray()
        value = 0
        bits = 0
        for i, pixel in enumerate(data):
            if i % self.width == 0:
                if bits:
                    packed.append(value << (8 - bits))
                    value = 0
                    bits = 0
                packed.append(0)  # No filter
            if self.color in [COLOR_RGB, COLOR_GRAYA, COLOR_RGBA]:
                for x in pixel:
                    value <<= self.depth
                    value |= x & ((1 << self.depth) - 1)
                    bits += self.depth
            else:
                value <<= self.depth
                value |= pixel & ((1 << self.depth) - 1)
                bits += self.depth
            while bits >= 8:
                bits -= 8
                packed.append((value >> bits) & 0xFF)
                value &= (1 << bits) - 1
        if bits:
            packed.append(value << (8 - bits))

        # Compress the byte array (no compression)
        comp_size = len(packed) + len(packed) // 0xffff * 5 + 5
        compressed = bytearray(comp_size)
        s1 = 1
        s2 = 0
        x = 0
        for i, byte in enumerate(packed):
            if i % 0xffff == 0:
                n = len(packed) - i
                if n > 0xffff:
                    n = 0xffff
                    compressed[x] = 0
                else:
                    compressed[x] = 1
                compressed[x+1:x+5] = (n.to_bytes(2, "little") +
                                       (n ^ 0xffff).to_bytes(2, "little"))
                x += 5

            s1 = (s1 + byte) % 65521
            s2 = (s2 + s1) % 65521

            compressed[x] = byte
            x += 1

        chunk = b"IDAT"
        chunk += bytes((0x78, 1))  # Deflate, compression level 0
        chunk += compressed
        chunk += ((s2 << 16) + s1).to_bytes(4, "big")
        self.write_chunk(chunk)

    def end(self):
        self.write_chunk(b"IEND")
        self.file.close()
