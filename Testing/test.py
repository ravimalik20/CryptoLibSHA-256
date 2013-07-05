import os

files_to_compile=['generate_hash_string','generate_hash_file']

# Compiling Files if not Compiled.
for i in files_to_compile:
	if os.path.exists(i):
		print "File %s.c already compiled."%i
	else:
		os.system("gcc -o %s %s.c"%(i,i))


# Declaring Test Cases:

test_hash_file={'TestCases/1':'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855',
	'TestCases/2':'edeaaff3f1774ad2888673770c6d64097e391bc362d7d6fb34982ddf0efd18cb',
	'TestCases/3':'7eb61810316c9f0c9a839b1dc619ac2097c8f808c37e4f6112b3bfa3dec401f4',
	'TestCases/4':'62859ed9427511e49b0e4b23a1551d838d8314e8dad60a8a0ce61e98fb74d639',
	'TestCases/5':'23cde71f9494f837f3e1ee7f6a5b8f646a73d30dcbb625250d8724e177431523',
}

test_hash_string={'':'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855',
	'abc':'ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad',
	'message digest':'f7846f55cf23e14eebeab5b4e1550cad5b509e3348fbc4efa3a1413d393cb650',
	'secure hash algorithm':'f30ceb2bb2829e79e4ca9753d35a8ecc00262d164cc077080295381cbd643f0d',
	'SHA256 is considered to be safe':'6819d915c73f4d1e77e4e1b52d1fa0f9cf9beaead3939f15874bd988e2a23630',
	'abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq':'248d6a61d20638b8e5c026930c3e6039a33ce45964ff2167f6ecedd419db06c1',
	'For this sample, this 63-byte string will be used as input data':'f08a78cbbaee082b052ae0708f32fa1e50c5c421aa772ba5dbb406a2ea6be342',
	'This is exactly 64 bytes long, not counting the terminating byte':'ab64eff7e88e2e46165e29f2bce41826bd4c7b3552f6b382a9e7d3af47c245f8'
	}

print "\n\nNow testing hash_file.h\n\n"

for i in test_hash_file:
	if os.path.exists(i):
		print "Test Case:%s"%i
		print "Hash Generated is:"
		os.system('./generate_hash_file "%s"'%i)
		print "Hash Should be:"
		print test_hash_file[i]
	else:
		print "File %s missing!!! Cannot operate test using it."%i

print "\n\nNow testing hash_string.h\n\n"

for i in test_hash_string:
	print "Test Case:%s"%(i)
	print "Hash Generated is:"
	os.system('./generate_hash_string "%s"'%i)
	print "Hash Should be:"
	print test_hash_string[i]

print "\n\nTEST COMPLETE!!\n\n"
