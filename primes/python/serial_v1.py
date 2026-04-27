# Example source code showing parallel processing in a serial workload.
#
# The source code is included as-is with no warranty.
#
import queue
import asyncio

async def send_log (q):
    while True:
        if not q.empty():
            msg = q.get()
            if msg == None:
                break;
            print(msg)
            # Other processing e.g. log to disk etc.
        else:
            # 250ms sleep between scans
            await asyncio.sleep(0.25)

async def main ():
    # Create the result queue
    log_q = queue.Queue()

    # Start the logger
    logger = asyncio.create_task(send_log(log_q))

    # Do something
    for i in range(0, 5):
        print(f"Iteration {i + 1}")

        # Output a log
        log_q.put(f"Log message {i + 1}")

        # Do some work
        fact = 1
        for f in range(1, (i + 1) * 10):
            fact *= i

        print(f"The factorial is {fact}")

    # Close down the logger
    log_q.put(None)
    await logger

    print("Job done") 

if __name__ == "__main__":
    asyncio.run(main())

