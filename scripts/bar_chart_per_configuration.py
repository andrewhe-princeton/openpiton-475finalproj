import matplotlib.pyplot as plt
import numpy as np
import os
import re

# path to the folder containing LFU, LRU, RAND, FIFO.... CHANGE!!
base_path = "../../FinalProjectEval_COS475"

output_folder = "images"
os.makedirs(output_folder, exist_ok=True)

# Configuration and Label Setup
x_labels = ['4-way 128KB', '4-way, 256KB', '4-way 512KB', '8-way 128KB','8-way, 256KB', '8-way 512KB']
x_label_keys = ['128kb_4way', '256kb_4way', '512kb_4way', '128kb_8way', '256kb_8way', '512kb_8way']
policy_labels = ['FIFO', 'LRU', 'LFU', 'RANDOM']
colors = ['purple', 'blue', 'green', 'orange']
benchmarks = ['binsearch', 'mibench_crc', 'mibench_stringsearch']
benchmarks_clean = ['binsearch', 'crc', 'stringsearch']
metrics = ['cycle count', 'hit rate']

def extract_cycle_count(file_path):
    try:
        with open(file_path, "r") as f:
            for line in f:
                if "ExecCyc=" in line:
                    match = re.search(r"ExecCyc=\s*(\d+)", line)
                    if match:
                        return int(match.group(1))
    except Exception:
        pass
    return None

def extract_hit_rate(file_path):
    try:
        with open(file_path, "r") as f:
            for line in f:
                if "L2 Hit Rate" in line:
                    match = re.search(r"L2 Hit Rate:\s+([\d.]+)%", line)
                    if match:
                        return float(match.group(1))
    except Exception:
        pass
    return None

replacement_policies = {
    'FIFO': 0,
    'LRU': 1,
    'LFU': 2,
    'RAND': 3
}

data_by_metric = [[[[None for _ in benchmarks] for _ in policy_labels] for _ in x_labels] for _ in metrics]

data_save_path = os.path.join("data")
os.makedirs(data_save_path, exist_ok=True)

np.save(os.path.join(data_save_path, "data_by_metric.npy"), np.array(data_by_metric, dtype=object))

print(f"Data saved into 'data/' folder for inspection and debugging.")

# === DATA COLLECTION ===
for policy_idx, (policy_folder, internal_policy_index) in enumerate(replacement_policies.items()):
    policy_path = os.path.join(base_path, policy_folder)
    for bench_idx, bench in enumerate(benchmarks):
        bench_path = os.path.join(policy_path, bench)
        for config_idx, config_folder in enumerate(x_label_keys):
            config_path = os.path.join(bench_path, config_folder)
            status_log_path = os.path.join(config_path, "status.log")
            hit_rate_path = os.path.join(config_path, config_folder)

            cycle = extract_cycle_count(status_log_path)
            hit = extract_hit_rate(hit_rate_path)

            data_by_metric[0][config_idx][policy_idx][bench_idx] = cycle
            data_by_metric[1][config_idx][policy_idx][bench_idx] = hit

# === PLOTTING ===
for metric_idx, metric in enumerate(metrics):
    for bench_idx, benchmark in enumerate(benchmarks_clean):
        fig, ax = plt.subplots(figsize=(12, 6))
        num_policies = len(policy_labels)
        bar_width = 0.8 / num_policies
        group_spacing = 1.0
        index = np.arange(len(x_labels)) * group_spacing

        for policy_idx, label in enumerate(policy_labels):
            values = []
            for config_idx in range(len(x_labels)):
                val = data_by_metric[metric_idx][config_idx][policy_idx][bench_idx]
                if val is None:
                    print(f"Warning: Missing data for {benchmark}, {metric}, {x_labels[config_idx]}, {label}")
                values.append(val if val is not None else 0)

            bars = ax.bar(index + policy_idx * bar_width, values, bar_width,
                          label=label, color=colors[policy_idx], edgecolor='black')

            # text lbels
            if metric == "hit rate":
                for bar, value in zip(bars, values):
                    ax.text(bar.get_x() + bar.get_width() / 2, bar.get_height() + 0.5,
                            f'{value:.1f}', ha='center', va='bottom', fontsize=8)

        ax.set_xlabel('Cache Configuration')
        ax.set_ylabel(metric.capitalize())
        ax.set_title(f'{benchmark.capitalize()} Benchmark - {metric.capitalize()} Comparison ({" vs ".join(policy_labels)})')
        ax.set_xticks(index + bar_width * (len(policy_labels) - 1) / 2)
        ax.set_xticklabels(x_labels, rotation=45, ha="right")

        # Legend in top-left corner
        ax.legend(title="Policy", loc='upper left', fontsize='small', title_fontsize='small')
        ax.grid(axis='y', linestyle='--', linewidth=0.5)

        plt.tight_layout()
        filename = f"{benchmark}_{metric.replace(' ', '_')}.png"
        plt.savefig(os.path.join(output_folder, filename))
        plt.close()

print(f"{len(metrics) * len(benchmarks)} benchmark-centric graphs saved in '{output_folder}'.")
