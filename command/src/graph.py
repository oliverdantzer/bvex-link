from matplotlib.figure import Figure

class Graph:
    def __init__(self, title, x_title, y_title):
        # Create the figure and axis
        self.fig = Figure(figsize=(5, 4), dpi=100)
        self.ax = self.fig.add_subplot(111)
        
        self.ax.set_title(title)
        self.ax.set_xlabel(x_title)
        self.ax.set_ylabel(y_title)
        self.ax.legend()

    def set_data(self, x, y):
        """Update the plot with new data."""
        self.ax.clear()
        self.ax.plot(x, y)