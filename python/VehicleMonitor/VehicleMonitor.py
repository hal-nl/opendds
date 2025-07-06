#!/usr/bin/env python3
# -*- coding: iso-8859-1 -*-

'''
Vehicle Monitor
'''

import sys
import time

from ctypes import *

import tkinter as tk

from tkinter import Button
from tkinter import Canvas
from tkinter import messagebox

LIBVEHICLE_DLL_PATH = "./libVehicle.so"
# LIBVEHICLE_DLL_PATH = "${DEPLOYPATH/libVehicle.so"
vehicleDLL = None

# -----------------------------------------------------
# Vehicles positions as dictionary
# -----------------------------------------------------
vehicleData = {}


# -----------------------------------------------------
# Colors of vehiclePosition
# -----------------------------------------------------
NUM_COLORS = 10
colors = ['black', 'brown', 'red', 'orange', 'yellow', 'green', 'blue', 'violet', 'gray', 'white']

# -----------------------------------------------------
# Constants
# -----------------------------------------------------
APP_TITLE="Vehicle Monitor"
ABOUT_GEOMETRY="400x400+100+100"
ABOUT_IMAGE="VehicleMonitor.png"

# -----------------------------------------------------
# Objects on canvas
# -----------------------------------------------------
canvas_ids = []


# -----------------------------------------------------
# Main application
# -----------------------------------------------------
app = tk.Tk()


# -----------------------------------------------------
# Add menu bar
# -----------------------------------------------------
main_menu = tk.Menu(app)
app.config(menu=main_menu)

# -----------------------------------------------------
# Add filemenu item
# -----------------------------------------------------
filemenu = tk.Menu(main_menu, tearoff=0)
filemenu.add_command(label="Quit", command=lambda text="Quit": OnMenuClick(text))
main_menu.add_cascade(label="File", menu=filemenu)

# -----------------------------------------------------
# Add helpmenu item
# -----------------------------------------------------
helpmenu = tk.Menu(main_menu, tearoff=0)
helpmenu.add_command(label="About", command=lambda text="About": OnMenuClick(text))
main_menu.add_cascade(label="Help", menu=helpmenu)

# -----------------------------------------------------
# Canvas
# -----------------------------------------------------
canvas = Canvas(width=800,height=800, bg="#cedbd2")
canvas.grid(row=2,column=0)


# -----------------------------------------------------
# Drawing the circle
# -----------------------------------------------------
def circle():
    x=100
    y=100
    r = 5
    canvas_id = canvas.create_oval(x-r,y-r,x+r,y+r)
    return canvas_id

# -----------------------------------------------------
# Drawing a cicle
# -----------------------------------------------------
def draw_circle(posx, posy, radius):
    canvas.delete("all")
    posx += 100
    posy += 100
    canvas_id = canvas.create_oval(posx-radius, posy-radius, posx+radius, posy+radius)
    return canvas_id

def QuitHandler():
    '''
    Application 'quit' Handler
    '''
    if messagebox.askyesno("Close " + APP_TITLE + "?", "Are you sure?"):
        print("Button click: quit YES", 'EVENT')
        if (vehicleDLL):
          vehicleDLL.VehicleDestroy()
        sys.exit(1)
    else:
        print("Button click: quit NO", 'EVENT')

# -----------------------------------------------------
# OnMenuClick
# -----------------------------------------------------
def OnMenuClick(text):
    message = "You selected menu item %s" % (text)

    if text == "About":
        print(message)
        AboutBox()
    elif text == "Quit":
        print(message)
        QuitHandler()
    else:
        message = "You selected menu item %s %s" % (text, "(Not implemented yet)")
        print(message)

# -----------------------------------------------------
# About box
# -----------------------------------------------------
def AboutBox():
    '''
    Show an about box window
    '''
    win = tk.Toplevel()
    # Show messages
    logo = tk.PhotoImage(file=ABOUT_IMAGE)
    label1 = tk.Label(win, image=logo, bd=-2)
    label1.image = logo # keep a reference!
    label1.pack()

    message3 = "About " + APP_TITLE + "\n"
    message4 = "Version 0.1\nCopyright (c) H.Ates 2021"

    label3 = tk.Label(win, text=message3, anchor="w", bg="White")
    label3.pack()

    label4 = tk.Label(win, text=message4, anchor="w", bg="White")
    label4.pack()

    win.title("About " + APP_TITLE + " Tool")
    win.configure(bg="White")

    win.geometry(ABOUT_GEOMETRY)


def draw_circles(radius):
    canvas.delete("all")
    index = 0
    for uuid in vehicleData:
        col = colors[index % NUM_COLORS]
        x = 100 + int(vehicleData[uuid]['x'])
        y = 100 + int(vehicleData[uuid]['y'])
        id = canvas.create_oval(x-radius, y-radius, x+radius, y+radius, outline=col)
        name = vehicleData[uuid]['name']
        id = canvas.create_text(x, y-10, fill=col, font="Times 8 italic bold", text=name)
        index += 1
    canvas.update

class VEHICLE_DATA(Structure):
    '''
    The structure for the data
    '''
    _fields_ = [
      ("index",         c_long  ),
      ("type",          c_long  ),
      ("ip_port",       c_long  ),
      ("ip",            c_long  ),
      ("velocity",      c_float ),
      ("acceleration",  c_float ),
      ("latitude",      c_float ),
      ("longitude",     c_float ),
      ("altitude",      c_float ),
      ("name",          c_char * 40),
      ("uuid",          c_char_p),
    ]


def infoCallBack(data):
    #print(data)
    pass

def dataReaderListenerCallback(data):
    x = float(data.contents.latitude)
    y = float(data.contents.longitude)
    uuid = str(data.contents.uuid)
    name = data.contents.name
    vehicleData[uuid] = {'name': name, 'x' : x, 'y' : y}
    message = "READER RECEIVED From :: {0}, Index={1}, [Lat, Lon]=[{2}, {3}]".format(name, data.contents.index, x, y)
    print(message)
    try :
      draw_circles(3)
    except:
      pass


# -----------------------------------------------------
# The first argument is the result type of the callback
# The rest are the arguments
# -----------------------------------------------------
DATARECEIVER_CALLBACK_FUNC = CFUNCTYPE(c_void_p, POINTER(VEHICLE_DATA))
reader_callback_func = DATARECEIVER_CALLBACK_FUNC(dataReaderListenerCallback)

INFO_CALLBACK_FUNC = CFUNCTYPE(c_void_p, c_char_p)
info_callback_func = INFO_CALLBACK_FUNC(infoCallBack)

# -----------------------------------------------------
# Main Start
# -----------------------------------------------------
if __name__ == '__main__':

    # -----------------------------------------------------
    # Load DLL
    # -----------------------------------------------------
    vehicleDLL = cdll.LoadLibrary(LIBVEHICLE_DLL_PATH)

    ARGUMENTS = b"VehicleMonitor.py -DCPSInfoRepo 127.0.0.1:12345 -ORBDebugLevel 10 -ORBLogFile VehicleMonitorPy.log --ddsconfig=/usr/local/apps/bin/debug/VehicleDDS.xml --monitor=false --name=PYTHON"
    p = create_string_buffer(ARGUMENTS)

    retValue = vehicleDLL.VehicleDataCreateArgv(info_callback_func, p)

    if retValue == 0:
      # -----------------------------------------------------
      # Delays for 1 seconds, to give time to the readers
      # to initialize their readers
      # -----------------------------------------------------
      time.sleep(1)
      retValue = vehicleDLL.VehicleDataReaderListener(reader_callback_func, info_callback_func)

    # -----------------------------------------------------
    # Main loop
    # -----------------------------------------------------
    app.title(APP_TITLE)

    app.config(menu=main_menu)
    #app.protocol("WM_DELETE_WINDOW", QuitHandler)
    app.mainloop()
    vehicleDLL.VehicleDataDestroy()
