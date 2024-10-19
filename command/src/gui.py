import tkinter as tk
from typing import TypeVar, Generic
import math
import threading
import time
import random

T = TypeVar('T')


class SliderValue(Generic[T]):
    def __init__(self, range: tuple, default: T, label: str, units: str | None = None):
        self.range = range
        self.current = default
        self.label = label
        self.units = units


sliderValues: set[SliderValue[float]] = {
    SliderValue[float]((1/60, 2), 2/60, 'temp', 'Hz'),
}


class GUI:
    def __init__(self):
        # Create the main window
        self.root = tk.Tk()
        self.root.title("Min. sample freq.")
        self.seq_nums = set()
        self.missing_seq_nums = set()
        self.total_num_segments = -1
        
        self.pixel_size = 20
        self.grid_size = (1, 1)
        # Create a canvas for the grid
        self.canvas = tk.Canvas(self.root, width=1, height=1, bg="white")
        self.canvas.pack(pady=20)
    
    def add_seq_num(self, seq_num: int):
        self.seq_nums.add(seq_num)
        self.draw_seq_num(seq_num, "green")
    
    def add_missing_seq_num(self, seq_num: int):
        self.missing_seq_nums.add(seq_num)
        self.draw_seq_num(seq_num, "red")
    
    def set_total_num_segments(self, total_num_segments: int):
        self.total_num_segments = total_num_segments
        grid_width = math.ceil(self.total_num_segments ** 0.5)
        self.grid_size = (grid_width, grid_width)
        self.canvas.config(width=self.grid_size[0] * self.pixel_size, height=self.grid_size[1] * self.pixel_size)

    
    def draw_grid_pixel(self, x, y, fill):
        
                self.canvas.create_rectangle(x, y, x + self.pixel_size, y + self.pixel_size, fill=fill, outline="gray")
    
    def draw_seq_num(self, seq_num: int, fill):
        if self.total_num_segments <= 0:
            return

        grid_x = (seq_num % self.grid_size[0])
        grid_y = (seq_num // self.grid_size[0])
        x = grid_x * self.pixel_size
        y = grid_y * self.pixel_size
        self.draw_grid_pixel(x, y, fill)


# Testing GUI
if __name__ == "__main__":
    
    def add_sequence_numbers(gui):
        i = 0
        while i < 100:
            gui.add_seq_num(i)
            time.sleep(0.1)
            i += random.randint(1, 2)
    gui = GUI()
    gui.set_total_num_segments(120)
    
    # Start adding sequence numbers in a separate thread
    threading.Thread(target=add_sequence_numbers, args=(gui,)).start()
    
    gui.root.mainloop()