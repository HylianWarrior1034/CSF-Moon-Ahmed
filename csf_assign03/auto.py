import subprocess

strategy = ["write-allocate write-back", "no-write-allocate write-through"]
evict = ["lru", "fifo"]
sizes = ["4", "256"]
mapping = ["1024 1", "256 4", "1 1024"]

for s in strategy:
    for e in evict:
        for size in sizes:
            for m in mapping:
                bashCommand = f"./csim {m} {size} {s} {e}"
                myinput = open("gcc.trace")
                process = subprocess.Popen(bashCommand.split(), stdin=myinput, stdout=subprocess.PIPE)
                output, error = process.communicate()
                print(f"{m} {size} {s} {e}")
                print(output.decode("utf-8"))
                print()
                

