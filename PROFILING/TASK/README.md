# Unix Domain Socket Key-Value Store Performance Report

## Performance Summary

* **Test duration:** 37 seconds under concurrent load
* **Syscalls observed (uds_kv_server_perf_top):**

  * `read`  : 5
  * `write` : 6
  * `accept`: 2

* **Test duration:** 25 seconds under concurrent load
* **Syscalls observed (uds_kv_client_perf_top):**

  * `read`  : 9
  * `write` : 12
  * `accept`: 0

* **Observations:**

  * A lot of `read` and `write` calls show the server spends most of its time on **I/O**.
  * `accept` shows how many **clients connected** to the server.
  * When many clients connect at the same time, threads may **wait on locks**, which can slow down writes.

## Load Scenarios

### 1. Read-heavy Load

* **State:** Many clients performing `GET` operations simultaneously.
* **Observation:** Server handles reads quickly; very little waiting.
* **Fix / Optimization:** No major fix needed; consider caching if read volume grows further.

### 2. Write-heavy Load

* **State:** Many clients performing `SET` operations.
* **Observation:** Threads sometimes wait on locks `mutex`, slowing writes.
* **Fix / Optimization:** Use smaller locks or lock-free structures to reduce waiting.

### 3. Mixed Load

* **State:** Clients do both `GET` and `SET`.
* **Observation:** Some delays from locks; moderate speed.
* **Fix / Optimization:** Optimize critical sections; minimize lock duration.

### 4. High Concurrency

* **State:** Many clients connecting simultaneously.
* **Observation:** The server makes a lot of accept calls and threads wait on locks, which can slow things down.
* **Fix / Optimization:** Consider using a **thread pool** instead of one thread per client, or use **asynchronous I/O** to handle many clients efficiently without blocking.

## Commands Used

### 1. Compile the server/client

gcc uds_kv_server_perf_top.c -o uds_kv_server_perf_top  
gcc uds_kv_client_perf_top.c -o uds_kv_client_perf_top

### 2. Run the server
./uds_kv_server_perf_top

### 3. Run multiple clients manually
./uds_kv_client_perf_top  # Terminal 1<br>
./uds_kv_client_perf_top  # Terminal 2

### 4. Profile the server/client with `perf`
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./uds_kv_server_perf_top  
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./uds_kv_client_perf_top

### 5. (Optional) View server PID manually
ps aux | grep uds_kv_server_perf_top

### 6. Capture screenshot 
# WSL: Windows + Shift + S
# Linux GUI users: PrtScn or gnome-screenshot -a

## Notes

* This report **excludes server/client code**.
* Designed to show **system behavior under different loads** and **bottleneck analysis**.
* Provides insights for **optimizing thread-based key-value store** performance.
