### Libraries
import tkinter
from tkinter import *
from tkinter import messagebox
from tkinter import scrolledtext

import string
import socket
import sys
import os
import threading



class Window(Frame):
    # Initialization settings
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.master = master
        self.trace_data = StringVar()
        self.sock_txt = scrolledtext.ScrolledText(
            self.master,
            width = 499,
            height = 20)
        self.ram_txt = scrolledtext.ScrolledText(
            self.master,
            width = 499,
            height = 20)
        self.init_window()

    # Initialiation of components
    def init_window(self):
        # title
        self.master.title("MOS 6502 Debugger")
        self.pack(expand=True,fill=BOTH)

        # crate menubar instance
        menu = Menu(self.master)
        self.master.config(menu=menu)

        # File submenu
        file = Menu(menu)
        file.add_command(label="Exit", command=self.client_exit)
        menu.add_cascade(label="File", menu=file)

        # Help submenu
        help = Menu(menu)
        help.add_command(label="About", command=self.about_msg_box)
        menu.add_cascade(label="Help", menu=help)

        # Text box
        self.sock_txt.pack(fill='y', expand = True)
        self.ram_txt.pack(fill='y', expand = True)

    def client_exit(self):
        exit()

    def about_msg_box(self):
        messagebox.showinfo("About","MOS 6502 debugger GUI.\n   Emanuel Aracena")


def communicate(app):
    # Check if server addr exists
    server_addr = './emulator_comm_socket'
    try:
        os.unlink(server_addr)
    except:
        if os.path.exists(server_addr):
            raise
            
    # Create UDS socket
    sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        
    # Bind socket to the address
    print ("|~| Starting up on %s\n" % server_addr)
    sock.bind(server_addr)

    # Listen for incoming connections
    sock.listen(1)

    # Wait for connection
    while True:
        print("|~| Waiting for a connection...")
        connection, client_addr = sock.accept()
        try:
            print("|+| Connection from client.")
            app.sock_txt.insert('end', "|+| Connection from client.\n")
            # Retrieve the data in small chunks and print to text box live
            # reregister callback, with internal delay
            retrieve_data(app, connection)
        finally:
            # Clean up the connection
            connection.close()

def retrieve_data(app, connection):                 
    app.update()
    data = connection.recv(100)
    data = data.decode('ASCII')
    app.trace_data.set(data)
    app.sock_txt.insert('end', app.trace_data.get() + '\n')
    app.sock_txt.update_idletasks()
    app.sock_txt.see("end")
    app.after(1000, retrieve_data(app, connection))
   
        
def update_ram_state(app):
    ram_snapshot = open("6502_RAM_SNAPSHOT.txt", "r")
    app.ram_txt.insert('end', ram_snapshot.read())

root = Tk()
root.geometry("500x800")
app = Window(root)

# set up communication and start communcation loop
app.after(1000, update_ram_state(app))
app.after(1000, communicate(app))
# mainloop
root.mainloop()
