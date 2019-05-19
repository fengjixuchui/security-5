# security
ctf, binary utils, debugging, notes

## utils

* aesSource : I implemented AES from source (and by lookup table).  Wanted a
  better understanding of how it worked and stuff for when I perform Reverse
  Engineering steps.

* simpleFileEncryption : Makes is easy to use OpenSSL to encrypt files.
  Behaves like a copy command for a single file at a time.  Works on both
  Linux and Windows.

* entropy : Two python utilities that were posted on the internet (not my own
  source) for determing entropy of a file (and a good indicator if a file is
  compressed or encrypted).  I realized after making this, binwalk has a -E 
  filename option that is great, graphs entropy and everything.

* FindFloats: Tool that will take raw hex, ignores extra non-hex characters,
  and dumps out all the possible values the hex could be converted to.  For
  instance, the hex below contained a constant of pi in it

![Find Floats](utils/FindFloats/FindFloatsScreenshot.png)

* ReverseCRC: Tool that will brute force determine all the possible CRC-8
  and CRC-16 seed values for an arbitrary blob of hexadecimal data.

![Reverse CRC](utils/ReverseCRC/Screenshot.png)

* xorFiles: xor the contents of 2 files into each other

