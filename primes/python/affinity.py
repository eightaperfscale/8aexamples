# Example source code to filter the fastest physical cores on a platform
#
# The source code is included as-is with no warranty.
#
import psutil
import pyhwloc

class platform_affinity:
    def __init__ (self):
        self.topology = pyhwloc.from_this_system().load()

    def find_best_cores (self):
        cores = []
        # Filter additional threads from physical cores
        for core in self.topology.iter_cores():
            first_child = next(core.iter_children())
            c = str(first_child).replace("PU#", "")
            cores.append(int(c))

        # Now we need to look at all frequencies
        low_freq = 100000
        high_freq = 0
        frequencies = psutil.cpu_freq(percpu=True)
        for i, freq in enumerate(frequencies):
            if not i in cores:
                continue

            low_freq = min(low_freq, freq.max)
            high_freq = max(high_freq, freq.max)

            if low_freq != high_freq:
                # Remove low frequency cores
                for i, freq in enumerate(frequencies):
                    if freq.max == low_freq:
                        cores.remove(i)

        return cores

