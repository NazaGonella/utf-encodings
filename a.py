c = "é"
c_utf = c.encode("utf-8")
print("byte length", len(c_utf))    # output: byte length 2
print(c_utf)                        # output: b'\xc3\xa9'

c = "é"
c_utf = c.encode("utf-8")
print("byte length", len(c_utf))    # output: byte length 3
print(c_utf)                        # output: b'e\xcc\x81'