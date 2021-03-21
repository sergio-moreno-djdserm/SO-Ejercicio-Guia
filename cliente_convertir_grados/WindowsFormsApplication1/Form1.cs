﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server; // Declarar un objeto de la clase socket
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
  
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                if (Celsius.Checked)
                {
                    // Enviar los grados Celsius y devolverlos en Fahrenheit
                    string mensaje = "1/" + cel.Text;
                    // Enviamos al servidor
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje); //Envio un vector de bytes
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80]; //Tambien sera un vector de bytes la respuesta
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0]; //Cortar por final de string
                    MessageBox.Show("Los grados Celsius convertidos en Fahrenheit son: " + mensaje);
                }

                else if (Fah.Checked)
                {
                    // Enviar los grados Fahrenheit y devolverlo en Celsius
                    string mensaje = "2/" + fahren.Text;
                    // Enviamos al servidor
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);

                    //Recibimos la respuesta del servidor
                    byte[] msg2 = new byte[80];
                    server.Receive(msg2);
                    mensaje = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                    MessageBox.Show("Los grados Fahrenheit convertidos en Celsius son: " + mensaje);
                }
            }
            catch
            {
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Creamos un IPEndPoint con el ip del servidor y puerto del servidor 
            //al que deseamos conectarnos
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9080);

            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
                MessageBox.Show("Conectado");
            }
            catch (SocketException)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            } 
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //Enviar mensaje de desconexión
            string mensaje = "0/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Nos desconectamos
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }
    }
}
