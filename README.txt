SHA-256 implementation.

Author: Ravi Malik

Why this library exists?

The sole intention of creating this SHA-256 library was learning. I was making some projects which required the use of a cryptographic hashing algorithm. And AES is the best hahsing algorithm that i came across. And i knew that there are implementations of SHA available on the internet which are both super optimised and highly secure. But i decided to write my own because i wish to learn rather than using some ready made tool infront of me. In future this library would grow into an optimised and secure one. Many issues would arise which i will fix and others will help me fix and learn alongside as there would arise many situations which would test one's skills. And eventually this will rise up as an efficient implementation. 

Happy Learning :D

Information about various files:

1. LICENSE.txt : Contains the details of the license applied to this project.
2. sha.h : This file contains the business logic of the SHA-256 hashing algorithm.
3. hash_file.h : This file uses the sha.h file to provide functions to hash any file.
4. hash_string.h : This file uses the sha.h file to provide functions to hash a string.
5. Testing:
	5.1. generate_hash_file.c : A small interface around hash_file.h for testing purpose.
	5.2. generate_hash_string.c : A small interface around hash_string.h for testing purpose.
