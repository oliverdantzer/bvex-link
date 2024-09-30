import tkinter as tk
from typing import TypeVar, Generic

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

        # Create a label to display the slider values
        self.label = tk.Label(self.root, text="Move the sliders!")
        self.label.pack(pady=10)

        self.sliders = []
        for sliderValue in sliderValues:
            def update_current(current: str):
                sliderValue.current = float(current)
            slider = tk.Scale(
                self.root,
                from_=sliderValue.range[0],
                to=sliderValue.range[1],
                orient='horizontal',
                label=sliderValue.label,
                command=update_current
            )
            slider.pack(pady=10)
            self.sliders.append(slider)
        
        # Create a button to exit the application
        self.exit_button = tk.Button(
            self.root, text="Exit", command=self.root.quit)
        self.exit_button.pack(pady=20)