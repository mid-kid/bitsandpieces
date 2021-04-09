table = []
polynomial = 0xC387
for x in range(256):
    k = x
    crc = 0
    for y in range(8):
        if (crc ^ k) & 1:
            crc = (crc >> 1) ^ polynomial
        else:
            crc >>= 1
        k >>= 1
    table.append(crc & 0xFFFF)

def crc16(data):
    crc = 0xFEFE
    for byte in data:
        crc = table[(crc & 0xFF) ^ byte] ^ (crc >> 8)
    return crc ^ 0xFFFF
