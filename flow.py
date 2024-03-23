import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt
import numpy as np

def parse_flowmon(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()

    total_rx_packets = 0
    total_tx_packets = 0
    total_jitter_ns = 0
    total_delay_ns = 0
    total_rx_bytes = 0
    total_tx_bytes = 0
    total_time_ns = 0

    for flow in root.iter('Flow'):
        rx_packets = int(flow.get('rxPackets') or 0)
        tx_packets = int(flow.get('txPackets') or 0)
        rx_bytes = int(flow.get('rxBytes') or 0)
        tx_bytes = int(flow.get('txBytes') or 0)
        
        delay_ns = float(flow.get('delaySum')[:-2]) if flow.get('delaySum') else 0
        jitter_ns = float(flow.get('jitterSum')[:-2]) if flow.get('jitterSum') else 0
        total_rx_packets += rx_packets
        total_tx_packets += tx_packets
        total_rx_bytes += rx_bytes
        total_tx_bytes += tx_bytes
        total_delay_ns += delay_ns
        total_jitter_ns += jitter_ns
        
        time_first_rx_packet_ns = flow.get('timeFirstRxPacket')
        time_last_rx_packet_ns = flow.get('timeLastRxPacket')
        if time_first_rx_packet_ns and time_last_rx_packet_ns:
            time_first_rx_packet_ns = float(time_first_rx_packet_ns[:-2])
            time_last_rx_packet_ns = float(time_last_rx_packet_ns[:-2])
            flow_duration_ns = time_last_rx_packet_ns - time_first_rx_packet_ns
            total_time_ns += flow_duration_ns

    # Calculate metrics
    pdf = total_rx_packets / total_tx_packets if total_tx_packets > 0 else 0
    avg_e2e_delay = total_delay_ns / total_rx_packets if total_rx_packets > 0 else 0
    avg_jitter = total_jitter_ns / total_rx_packets if total_rx_packets > 0 else 0
    throughput = (total_rx_bytes * 8) / (total_time_ns / 1e9) if total_time_ns > 0 else 0
    overhead = total_tx_bytes - total_rx_bytes

    return {
        'pdf': pdf,
        'avg_e2e_delay_s': avg_e2e_delay / 1e9,  # Convert ns to s
        'avg_jitter_s': avg_jitter / 1e9,  # Convert ns to s
        'throughput_kbps': throughput / 1000,  # Convert bps to kbps
        'overhead_Mbytes': overhead / 1000
    }

metrics_75 = parse_flowmon('patrol-wildlife-75.flowmon')
metrics_150 = parse_flowmon('patrol-wildlife-150.flowmon')

# Ensure that both dictionaries have the same number of items


# Calculate overhead for both densities
# overhead_75 = metrics_75['overhead_bytes']
# overhead_150 = metrics_150['overhead_bytes']

# Define the width of the bars
bar_width = 0.35

# Create subplot
fig, ax1 = plt.subplots(figsize=(10, 6))

# Extract the metrics into lists, ensuring they have the same length
metrics_labels = list(metrics_75.keys())
values_75 = list(metrics_75.values())
values_150 = list(metrics_150.values())

# Check the lengths match, just to be sure
assert len(values_75) == len(values_150), "Length of values for 75 and 150 nodes does not match."

# Set the positions for the groups of bars
positions = np.arange(len(metrics_labels))

# Plotting the metrics for both densities
bars_75 = ax1.bar(positions - bar_width/2, values_75, bar_width, label='75 Nodes')
bars_150 = ax1.bar(positions + bar_width/2, values_150, bar_width, label='150 Nodes')

ax1.set_xlabel('Metrics')
ax1.set_ylabel('Values (log scale)')
ax1.set_title('Performance Metrics by Node Density (log scale for small values)')
ax1.set_xticks(positions)
ax1.set_xticklabels(list(metrics_75.keys()))
ax1.set_yscale('log')  # Set log scale for primary y-axis
ax1.legend()

# Plot the overhead on a secondary y-axis due to its larger scale
# ax2 = ax1.twinx()
# ax2.bar(positions - bar_width/2, overhead_75, bar_width, label='Overhead 75 Nodes', color='tab:orange', alpha=0.5)
# ax2.bar(positions + bar_width/2, overhead_150, bar_width, label='Overhead 150 Nodes', color='tab:brown', alpha=0.5)
# ax2.set_ylabel('Overhead Bytes')
# ax2.tick_params(axis='y', labelcolor='tab:brown')

# Function to automatically label the bars with their height values
def autolabel(bars):
    for bar in bars:
        height = bar.get_height()
        ax1.annotate('{:.3f}'.format(height),
                     xy=(bar.get_x() + bar.get_width() / 2, height),
                     xytext=(0, 3),  # 3 points vertical offset
                     textcoords="offset points",
                     ha='center', va='bottom')

# Call the autolabel function
autolabel(bars_75)
autolabel(bars_150)

fig.tight_layout()  # To make sure everything fits without overlap

# Include a legend
handles, labels = ax1.get_legend_handles_labels()
ax1.legend(handles , labels, loc='upper left')


# Display the plot
plt.savefig('flowmon-150-patrol-wildlife.png')