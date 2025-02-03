import sys
import argparse
from pmu import PMUdata
from counter import counter

def main():

    # Create ArgumentParser object
    parser = argparse.ArgumentParser(description='Set options for PMU analysis')
    parser.add_argument('filename', type=str, help='Input filename contains raw PMU data')
    parser.add_argument('--cpu', type=str, default='graviton3e', help='Print raw PMU data')
    parser.add_argument('--debug', action='store_true', help='Print raw PMU counter')
    parser.add_argument('--noheader', action='store_true', help='Not Print PMU header')
    args = parser.parse_args()

    # Create PMU object
    pmu = PMUdata(cpu_type=args.cpu)

    counter(pmu, args)

if __name__ == "__main__":
    main()
