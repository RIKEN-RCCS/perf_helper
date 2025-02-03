from gracecpu import gracecpu
from graviton3e import graviton3e
from fugaku import fugaku
from graviton4 import graviton4

# Measure performance break down

def counter(pmu, args):
    # Initialize variables
    events = len(pmu.list)

    # Open the CSV file and read its content
    with open(args.filename, mode='r') as file:
        for line in file:
            row = line.split()

            # Iterate through each row in the CSV
            # Skip empty lines and comments (lines starting with #)
            if not row or row[0].startswith('#') or not row[0].startswith('Measured'):
                continue

            for i in range(0,events):
                if row[3].lower() == 'l1i_cache_access':
                    row[3] = 'l1i_cache'
                if row[3].lower() == 'l2d_cache_access':
                    row[3] = 'l2d_cache'
                if pmu.list[i] == row[3].lower():
                    if pmu.value[i] == 0:
                        pmu.set_value(i, int(row[4]))

        if args.debug == True:
            for i in range(0,events):
                print(i, pmu.list[i], pmu.value[i])
        if pmu.CPU_TYPE == 'graviton3e':
            graviton3e(pmu, args)
        elif pmu.CPU_TYPE == 'gracecpu':
            gracecpu(pmu, args)
        elif pmu.CPU_TYPE == 'fugaku':
            fugaku(pmu, args)
        elif pmu.CPU_TYPE == 'graviton4':
            graviton4(pmu, args)
