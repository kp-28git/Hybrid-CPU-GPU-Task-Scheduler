import subprocess
import csv
import os
import re

build_dir = "build"
os.makedirs(build_dir, exist_ok=True)

print("Running CMake...")
subprocess.run(["cmake", "-S", ".", "-B", build_dir, "-G", "MinGW Makefiles"], check=True)

print("Running mingw32-make...")
subprocess.run(["cmake", "--build", build_dir], check=True)

print("Build finished!")

exe_path = r".\build\hybrid_scheduler_cpu.exe"
csv_file = "results.csv"

# Run executable 10 times
runs = 10
results = []

for i in range(runs):
    print(f"Run {i+1}/{runs}...")
    proc = subprocess.run(exe_path, capture_output=True, text=True, shell=True)
    output = proc.stdout

    # Extract policy
    policy_match = re.search(r"Scheduler created with policy:\s*(.*)", output)
    policy = policy_match.group(1).strip() if policy_match else "Unknown"

    # Extract total execution time (float in ms)
    time_match = re.search(r"Total execution time\s*:\s*Elapsed time:\s*([\d\.]+)", output)
    exec_time = float(time_match.group(1)) if time_match else None

    if exec_time is not None:
        results.append(exec_time)

# Step 3: Append results to CSV as new column
if results:
    # Read existing data if file exists
    data = []
    if os.path.isfile(csv_file):
        with open(csv_file, "r", newline="") as f:
            reader = list(csv.reader(f))
            data = reader

    # Ensure we have at least 10 rows
    while len(data) < runs + 1:  # +1 for header
        data.append([])

    # Insert header
    if len(data[0]) == 0:  # new file
        data[0].append(policy)
    else:
        data[0].append(policy)

    # Insert results in column
    for i, val in enumerate(results, start=1):
        # pad row if shorter
        while len(data[i]) < len(data[0]) - 1:
            data[i].append("")
        data[i].append(val)

    # Write back to CSV
    with open(csv_file, "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerows(data)

    print(f"Results saved in {csv_file}")