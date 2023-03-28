import subprocess

bashCommand = "CSF-Moon-Ahmed\csf_assign03\csim 256 4 16 write-allocate write-back lru < CSF-Moon-Ahmed\csf_assign03\gcc.trace"
process = subprocess.Popen(bashCommand.split(), stdout=subprocess.PIPE)
output, error = process.communicate()
print(output)