from binascii import crc32

def png_write_chunk(f, chunk):
    f.write((len(chunk) - 4).to_bytes(4, "big"))
    f.write(chunk)
    f.write(crc32(chunk).to_bytes(4, "big"))

def png_write_header(f, width, height, depth, color):
    # http://png-mng.sourceforge.net/pub/png/spec/1.2/png-1.2-pdg.html#C.IHDR
    f.write(b"\211PNG\r\n\032\n")
    chunk = b"IHDR"
    chunk += width.to_bytes(4, "big") + height.to_bytes(4, "big")
    chunk += bytes((depth, color, 0, 0, 0))
    png_write_chunk(f, chunk)

def png_write_palette(f, palette):
    chunk = bytearray(b"PLTE")
    for x in palette:
        chunk += bytes(x[:3])
    png_write_chunk(f, chunk)

def png_write_data(f, data, width, bpp):
    # Pack the image data into a byte array (no filtering)
    packed = bytearray()
    value = 0
    bits = 0
    for i, pixel in enumerate(data):
        if i % width == 0:
            if bits:
                packed.append(value << (8 - bits))
                value = 0
                bits = 0
            packed.append(0)  # No filter
        value <<= bpp
        value += pixel
        bits += bpp
        while bits >= 8:
            packed.append(value & 0xFF)
            value >>= 8
            bits -= 8
    if bits:
        packed.append(value << (8 - bits))

    # Compress the byte array (no compression)
    comp_sz = len(packed) + len(packed) // 0xffff * 5 + 5
    compressed = bytearray(comp_sz)
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
    png_write_chunk(png, chunk)

def png_end(f):
    png_write_chunk(f, b"IEND")
