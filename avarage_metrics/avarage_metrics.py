import subprocess
import re

exe_path = "C:\\Users\\puhinsky\\source\\repos\\n-body\\x64\\Debug\\n-body.exe"
argument = "10000"
times = 10
timings = list()

def launch_exe():
    result = subprocess.run([exe_path, argument], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    total_time_match = re.search(r'Total time: (\d+)', result.stdout)
    
    if total_time_match:
        total_time = int(total_time_match.group(1))
        print(f"Total time: {total_time} ms")
        timings.append(total_time);
    else:
        print("Total time not found")

for i in range(times):
    print(f"Launching {i+1}/{times}")
    launch_exe()
    print("=====================================")
    
print(f"Avarage: {sum(timings)/len(timings)} ms")

