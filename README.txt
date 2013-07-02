SHA-256 implementation.

Author: Ravi Malik

Why this library exists?

The sole intention of creating this SHA-256 library was learning. I was making some projects which required the use of a cryptographic hashing algorithm. And AES is the best hahsing algorithm that i came across. And i knew that there are implementations of SHA available on the internet which are both super optimised and highly secure. But i decided to write my own because i wish to learn rather than using some ready made tool infront of me. In future this library would grow into an optimised and secure one. Many issues would arise which i will fix and others will help me fix and learn alongside as there would arise many situations which would test one's skills. And eventually this will rise up as an efficient implementation. 

Happy Learning :D

Information about various files:

1. sha.h : This file contains the business logic of the SHA-256 hashing algorithm.
2. hash_file.h : This file uses the sha.h file to provide functions to hash any file.
3. test.c : This is the test file used to generate hash of a file and is build on the hash_file.h file.
