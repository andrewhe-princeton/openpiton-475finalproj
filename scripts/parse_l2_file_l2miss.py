import re
import sys

def analyze_l2_cache_log_revised(log_file_path):
    total_l2_outcomes_observed = 0
    l2_misses_observed = 0
    l2_hits_observed = 0 # total - misses

    # Looks for TILE0 L1.5 receiving/sending messages with l2miss status
    # value (0 or 1) after 'l2miss '
    l2_outcome_pattern = re.compile(r"l2_hit:\s*(\d)")
 
    # Simple check for L2 request initiation (less reliable for counting total accesses)
    # l2_request_pattern = re.compile(r"TILE0 L1\.5: Sending NOC1 .*_REQUEST")

    print(f"Processing log file: {log_file_path}...")
    line_count = 0
    try:
        with open(log_file_path, 'r') as f:
            for i, line in enumerate(f):
                line_count = i + 1
                # Check for outcome lines
                match = l2_outcome_pattern.search(line)
                if match:
                    total_l2_outcomes_observed += 1
                    miss_status = int(match.group(1)) # captured digit (0 or 1)
                    
                    if miss_status == 1:
                        l2_misses_observed += 1
                    # miss_status == 0 implies a hit for these messages
              


    except FileNotFoundError:
        print(f"Error: File not found at '{log_file_path}'", file=sys.stderr)
        print("Please ensure the file name and path are correct.", file=sys.stderr)
        return None
    except Exception as e:
        print(f"An error occurred during processing line {line_count}: {e}", file=sys.stderr)
        return None

    print(f"Processing complete. Total lines processed: {line_count}")

    if total_l2_outcomes_observed > 0:
        # Calculate hits based on observed outcomes
        l2_hits_observed = total_l2_outcomes_observed - l2_misses_observed
        miss_rate = (l2_misses_observed / total_l2_outcomes_observed) * 100
        
        return {
            "total_accesses_observed": total_l2_outcomes_observed,
            "hits": l2_misses_observed,
            "misses": l2_hits_observed, # Calculated
            "hit_rate": miss_rate
        }
    else:
        # No relevant L2 outcome lines found
        print("Warning: No lines matching the L2 outcome pattern were found.", file=sys.stderr)
        print("         Please double-check the log format and the script's regex.", file=sys.stderr)
        return {
            "total_accesses_observed": 0,
            "misses": 0,
            "hits": 0,
            "miss_rate": 0.0 
        }

if __name__ == "__main__":
    # Get log file path from command line argument or use default
    if len(sys.argv) > 1:
        log_file = sys.argv[1]
    else:
        log_file = 'sim.txt' #  back to sim.txt

    results = analyze_l2_cache_log_revised(log_file)

    if results:
        print("\n--- L2 Cache Statistics (Based on Return Messages) ---")
        print(f"Log File Analyzed:           {log_file}")
        print(f"Total L2 Outcomes Observed:  {results['total_accesses_observed']}")
        print(f"L2 Misses Observed:          {results['misses']}")
        print(f"L2 Hits Calculated:          {results['hits']}")
        if results['total_accesses_observed'] > 0:
            print(f"L2 Hit Rate:                 {results['hit_rate']:.2f}%")
        else:
            print("L2 Hit Rate:                 N/A (No L2 outcomes observed in log)")
            
        # Add a note if the result is zero
        if results['total_accesses_observed'] == 0 and 'sims.txt' in log_file:
             print("\nNOTE: The log file analyzed was 'sims.txt'. The original log provided was 'sim.txt'.")
             print("      If 'sims.txt' was unintentional or doesn't exist, please ensure you are using the correct file name ('sim.txt').")
        elif results['total_accesses_observed'] == 0:
             print("\nNOTE: No L2 accesses were detected based on the expected log pattern.")
             print("      Please verify the log file contents and the script's parsing logic if this is unexpected.")

    else:
        print("Could not generate statistics.")
