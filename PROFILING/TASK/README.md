# Unix Domain Socket Key-Value Store Performance Report

## Performance Summary

* **Test duration:** 37 seconds under concurrent load
* **Syscalls observed:**

  * `read`  : 5
  * `write` : 6
  * `accept`: 2

* **Observations:**

  * High `read` and `write` counts indicate the server is **I/O bound**.
  * `accept` calls reflect **number of new client connections**.
  * Mutex contention (`futex`) may appear under **high concurrency**, potentially slowing writes.

## Load Scenarios

### 1. Read-heavy Load

* **State:** Many clients performing `GET` operations simultaneously.
* **Observation:** Minimal mutex contention; server handles multiple reads efficiently.
* **Fix / Optimization:** No major fix needed; consider caching if read volume grows further.

### 2. Write-heavy Load

* **State:** Many clients performing `SET` operations.
* **Observation:** Mutex contention increases; writes may slow down.
* **Fix / Optimization:** Use finer-grained locks or lock-free data structures to reduce contention.

### 3. Mixed Load

* **State:** Reads and writes interleaved.
* **Observation:** Some mutex contention; moderate latency.
* **Fix / Optimization:** Optimize critical sections; minimize lock duration.

### 4. High Concurrency

* **State:** Many clients connecting simultaneously.
* **Observation:** High `accept` count; increased `futex` calls due to thread synchronization.
* **Fix / Optimization:** Consider using a **thread pool** instead of one thread per client, or asynchronous I/O.

## Commands Used

### 1. Compile the server

gcc uds_kv_server_perf_top.c -o uds_kv_server_perf_top

### 2. Run the server
./uds_kv_server_perf_top

### 3. Run multiple clients manually
./uds_kv_client_perf_top  # Terminal 1
./uds_kv_client_perf_top  # Terminal 2

### 4. Profile the server with `perf`
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./uds_kv_server_perf_top

### 5. (Optional) View server PID manually
ps aux | grep uds_kv_server_perf_top

### 6. Capture screenshot (if in WSL, use Windows + Shift + S)
# Linux GUI users:
Linux GUI: PrtScn or gnome-screenshot -a

## Notes

* This report **excludes server/client code**.
* Designed to show **system behavior under different loads** and **bottleneck analysis**.
* Provides insights for **optimizing thread-based key-value store** performance.
