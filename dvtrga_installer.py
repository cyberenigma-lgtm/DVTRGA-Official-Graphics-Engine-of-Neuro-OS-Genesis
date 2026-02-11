"""
DVTRGA Installer for Windows 11
Instalador profesional con GUI para el driver DVTRGA

Desarrollado por: José Manuel Moreno Cano
"""

import tkinter as tk
from tkinter import ttk, messagebox
import os
import subprocess
import sys
import ctypes

class DVTRGAInstaller:
    def __init__(self, root):
        self.root = root
        self.root.title("DVTRGA Driver Installer v1.0")
        self.root.geometry("600x500")
        self.root.resizable(False, False)
        
        # Verificar permisos de administrador
        if not self.is_admin():
            messagebox.showerror("Error", "Este instalador requiere permisos de administrador.\nEjecútalo como administrador.")
            sys.exit(1)
        
        self.create_widgets()
    
    def is_admin(self):
        """Verificar si se ejecuta como administrador"""
        try:
            return ctypes.windll.shell32.IsUserAnAdmin()
        except:
            return False
    
    def create_widgets(self):
        # Header
        header_frame = tk.Frame(self.root, bg="#0078D4", height=80)
        header_frame.pack(fill=tk.X)
        
        title = tk.Label(header_frame, text="DVTRGA Driver Installer", 
                        font=("Segoe UI", 20, "bold"), 
                        bg="#0078D4", fg="white")
        title.pack(pady=20)
        
        # Main content
        content_frame = tk.Frame(self.root, padx=20, pady=20)
        content_frame.pack(fill=tk.BOTH, expand=True)
        
        # Descripción
        desc = tk.Label(content_frame, 
                       text="DVTRGA - Direct Visual Transport & Render Graphics Architecture\n\n"
                            "Motor gráfico universal para Windows 11\n"
                            "Compatible con Intel, AMD, NVIDIA y emuladores",
                       font=("Segoe UI", 10),
                       justify=tk.LEFT)
        desc.pack(pady=10)
        
        # Componentes a instalar
        components_label = tk.Label(content_frame, 
                                   text="Componentes a instalar:",
                                   font=("Segoe UI", 11, "bold"))
        components_label.pack(anchor=tk.W, pady=(20, 5))
        
        self.install_driver = tk.BooleanVar(value=True)
        self.install_lib = tk.BooleanVar(value=True)
        self.install_test = tk.BooleanVar(value=True)
        
        tk.Checkbutton(content_frame, text="Driver modo kernel (dvtrga.sys)", 
                      variable=self.install_driver,
                      font=("Segoe UI", 10)).pack(anchor=tk.W, padx=20)
        
        tk.Checkbutton(content_frame, text="Biblioteca user-mode (dvtrga_api.dll)", 
                      variable=self.install_lib,
                      font=("Segoe UI", 10)).pack(anchor=tk.W, padx=20)
        
        tk.Checkbutton(content_frame, text="Aplicación de prueba (dvtrga_test.exe)", 
                      variable=self.install_test,
                      font=("Segoe UI", 10)).pack(anchor=tk.W, padx=20)
        
        # Barra de progreso
        self.progress = ttk.Progressbar(content_frame, mode='determinate', length=500)
        self.progress.pack(pady=20)
        
        # Status label
        self.status_label = tk.Label(content_frame, text="Listo para instalar", 
                                     font=("Segoe UI", 9),
                                     fg="green")
        self.status_label.pack()
        
        # Botones
        button_frame = tk.Frame(content_frame)
        button_frame.pack(pady=20)
        
        self.install_button = tk.Button(button_frame, text="Instalar", 
                                       command=self.install,
                                       bg="#0078D4", fg="white",
                                       font=("Segoe UI", 10, "bold"),
                                       width=15, height=2)
        self.install_button.pack(side=tk.LEFT, padx=5)
        
        self.cancel_button = tk.Button(button_frame, text="Cancelar", 
                                      command=self.root.quit,
                                      font=("Segoe UI", 10),
                                      width=15, height=2)
        self.cancel_button.pack(side=tk.LEFT, padx=5)
    
    def update_status(self, message, progress=0):
        """Actualizar estado y progreso"""
        self.status_label.config(text=message)
        self.progress['value'] = progress
        self.root.update()
    
    def install(self):
        """Proceso de instalación"""
        try:
            self.install_button.config(state=tk.DISABLED)
            
            # Paso 1: Verificar archivos
            self.update_status("Verificando archivos...", 10)
            if not self.verify_files():
                messagebox.showerror("Error", "Faltan archivos necesarios para la instalación")
                return
            
            # Paso 2: Instalar driver
            if self.install_driver.get():
                self.update_status("Instalando driver modo kernel...", 30)
                self.install_kernel_driver()
            
            # Paso 3: Instalar biblioteca
            if self.install_lib.get():
                self.update_status("Instalando biblioteca user-mode...", 60)
                self.install_user_library()
            
            # Paso 4: Instalar aplicación de prueba
            if self.install_test.get():
                self.update_status("Instalando aplicación de prueba...", 80)
                self.install_test_app()
            
            # Paso 5: Configurar sistema
            self.update_status("Configurando sistema...", 90)
            self.configure_system()
            
            # Completado
            self.update_status("Instalación completada exitosamente", 100)
            messagebox.showinfo("Éxito", 
                              "DVTRGA se instaló correctamente.\n\n"
                              "Reinicia el sistema para que los cambios tengan efecto.")
            
        except Exception as e:
            messagebox.showerror("Error", f"Error durante la instalación:\n{str(e)}")
        finally:
            self.install_button.config(state=tk.NORMAL)
    
    def verify_files(self):
        """Verificar que existen los archivos necesarios"""
        required_files = []
        
        if self.install_driver.get():
            required_files.append("dvtrga.sys")
            required_files.append("dvtrga.inf")
        
        if self.install_lib.get():
            required_files.append("dvtrga_api.dll")
        
        if self.install_test.get():
            required_files.append("dvtrga_test.exe")
        
        for file in required_files:
            if not os.path.exists(file):
                return False
        
        return True
    
    def install_kernel_driver(self):
        """Instalar driver modo kernel"""
        # Copiar archivos a System32\drivers
        drivers_path = r"C:\Windows\System32\drivers"
        
        subprocess.run([
            "copy", "dvtrga.sys", drivers_path
        ], shell=True, check=True)
        
        # Instalar driver con pnputil
        subprocess.run([
            "pnputil", "/add-driver", "dvtrga.inf", "/install"
        ], check=True)
        
        # Crear servicio
        subprocess.run([
            "sc", "create", "DVTRGA",
            "binPath=", r"C:\Windows\System32\drivers\dvtrga.sys",
            "type=", "kernel",
            "start=", "demand"
        ], check=True)
    
    def install_user_library(self):
        """Instalar biblioteca user-mode"""
        # Copiar DLL a System32
        subprocess.run([
            "copy", "dvtrga_api.dll", r"C:\Windows\System32"
        ], shell=True, check=True)
        
        # Registrar DLL
        subprocess.run([
            "regsvr32", "/s", r"C:\Windows\System32\dvtrga_api.dll"
        ], check=True)
    
    def install_test_app(self):
        """Instalar aplicación de prueba"""
        # Crear directorio en Program Files
        install_dir = r"C:\Program Files\DVTRGA"
        os.makedirs(install_dir, exist_ok=True)
        
        # Copiar ejecutable
        subprocess.run([
            "copy", "dvtrga_test.exe", install_dir
        ], shell=True, check=True)
        
        # Crear acceso directo en escritorio
        self.create_shortcut(
            os.path.join(install_dir, "dvtrga_test.exe"),
            os.path.join(os.path.expanduser("~"), "Desktop", "DVTRGA Test.lnk")
        )
    
    def configure_system(self):
        """Configurar sistema"""
        # Añadir entrada al registro
        subprocess.run([
            "reg", "add",
            r"HKLM\SOFTWARE\Neuro-OS\DVTRGA",
            "/v", "Version",
            "/t", "REG_SZ",
            "/d", "1.0",
            "/f"
        ], check=True)
    
    def create_shortcut(self, target, shortcut_path):
        """Crear acceso directo"""
        try:
            import win32com.client
            shell = win32com.client.Dispatch("WScript.Shell")
            shortcut = shell.CreateShortCut(shortcut_path)
            shortcut.Targetpath = target
            shortcut.WorkingDirectory = os.path.dirname(target)
            shortcut.IconLocation = target
            shortcut.save()
        except:
            pass  # Si falla, continuar sin acceso directo

def main():
    root = tk.Tk()
    app = DVTRGAInstaller(root)
    root.mainloop()

if __name__ == "__main__":
    main()
