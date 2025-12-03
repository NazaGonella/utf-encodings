c = "é"
c_utf = c.encode("utf-8")
print("byte length", len(c_utf))    # OUTPUT: byte length 2
print(c_utf)                        # OUTPUT: b'\xc3\xa9'

c = "é"
c_utf = c.encode("utf-8")
print("\nbyte length", len(c_utf))  # OUTPUT: byte length 3
print(c_utf)                        # OUTPUT: b'e\xcc\x81'