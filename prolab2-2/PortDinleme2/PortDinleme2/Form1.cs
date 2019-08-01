using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Collections;

namespace PortDinleme2
{
    public partial class Form1 : Form
    {

        string veri;
        ArrayList stoklar = new ArrayList();
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            veri = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(DisplayText));
        }

        private void DisplayText(object sender, EventArgs e)
        {
            stoklar.Add(veri);
            listBox1.Items.Clear();
           

            for (int i = 0; i < stoklar.Count; i += 1)
            {
                if(stoklar[i].ToString().Contains("E"))
                {
                    listBox1.Items.Add(stoklar[i].ToString());
                }
            }

            dosyayaYazdir();
            timer1.Start();
        }

        Random rnd = new Random();


        private void timer1_Tick(object sender, EventArgs e)
        {
           
        }

        private void dosyayaYazdir()
        {
            int yirmiBesIndeks, elliIndeks, tl1Indeks, suIndeks, cayIndeks, kahveIndeks,
                cikolataIndeks, biskuviIndeks;
            string temp = "";

            for (int i = stoklar.Count-1; i >= 0; i--)
            {
                if (stoklar[i].ToString().Contains("E"))
                {
                    temp = stoklar[i].ToString();
                }
            }
            
            if(temp.Length > 0)
            {
                yirmiBesIndeks = temp.IndexOf("Y");
                tl1Indeks = temp.IndexOf("T");
                suIndeks = temp.IndexOf("S");
                cayIndeks = temp.IndexOf("C");
                kahveIndeks = temp.IndexOf("K");
                cikolataIndeks = temp.IndexOf("c");
                biskuviIndeks = temp.IndexOf("B");

                string yirmiBes, elli, tl1, cay, cikolata, kahve, biskuvi, su;
                yirmiBes = elli = tl1 = cay = cikolata = kahve = biskuvi = su = "";
                
                yirmiBes = temp.Substring(0, yirmiBesIndeks);
                temp = temp.Substring(yirmiBesIndeks+1);

                elliIndeks = temp.IndexOf("E");
                elli = temp.Substring(0, elliIndeks);
                temp = temp.Substring(elliIndeks + 1);

                tl1Indeks = temp.IndexOf("T");
                tl1 = temp.Substring(0, tl1Indeks);
                temp = temp.Substring(tl1Indeks + 1);

                suIndeks = temp.IndexOf("S");
                su = temp.Substring(0, suIndeks);
                temp = temp.Substring(suIndeks+1);

                cayIndeks = temp.IndexOf("C");
                cay = temp.Substring(0, cayIndeks);
                temp = temp.Substring(cayIndeks + 1);

                kahveIndeks = temp.IndexOf("K");
                kahve = temp.Substring(0, kahveIndeks);
                temp = temp.Substring(kahveIndeks + 1);

                cikolataIndeks = temp.IndexOf("c");
                cikolata = temp.Substring(0, cikolataIndeks);
                temp = temp.Substring(cikolataIndeks + 1);

                biskuviIndeks = temp.IndexOf("B");
                biskuvi = temp.Substring(0, biskuviIndeks);

                biskuvi = biskuvi.Trim().ToString();

                //textBox1.Text = biskuvi;
                
                listBox1.Items.Add("yirmibes : " + yirmiBes.ToString());
                listBox1.Items.Add("elli : " + elli.ToString());
                listBox1.Items.Add("tl1 : "  +tl1.ToString());
                listBox1.Items.Add("su : " + su.ToString());
                listBox1.Items.Add("cay : " +cay.ToString());
                listBox1.Items.Add("kahve : " + kahve.ToString());
                listBox1.Items.Add("cikolata : " + cikolata.ToString());
                listBox1.Items.Add("biskuvi : " + biskuvi.ToString());

                string birinciSatir, ikinciSatir, ucuncuSatir, dorduncuSatir, besinciSatir, altinciSatir;

                birinciSatir = yirmiBes.Trim() + "," + elli.Trim() + "," + tl1.Trim();
                ikinciSatir = "1,su," + su.Trim() + ",50 Kurus";
                ucuncuSatir = "2,cay," + cay.Trim() + ",1 TL";
                dorduncuSatir = "3,kahve," + kahve.Trim() + ",1.5 TL";
                besinciSatir = "4,cikolata," + cikolata.Trim() + ",1.75 TL";
                altinciSatir = "5,biskuvi," + biskuvi.Trim() + ",2 TL";
                

                string dosya_yolu = @"C:\\Users\\Hacı Bayram ÜNAL\\Desktop\\urunler.txt";
                StreamWriter yaz = new StreamWriter(dosya_yolu);
                yaz.WriteLine(birinciSatir);
                yaz.WriteLine(ikinciSatir);
                yaz.WriteLine(ucuncuSatir);
                yaz.WriteLine(dorduncuSatir);
                yaz.WriteLine(besinciSatir);
                yaz.WriteLine(altinciSatir);

                yaz.Close();
            }
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = "COM3";
                serialPort1.BaudRate = 115200;
                serialPort1.DataBits = 8;
                serialPort1.Handshake = Handshake.None;

                serialPort1.Open();
                serialPort1.ReadTimeout = 500;

                serialPort1.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived);
               
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            stoklar.Clear();
            if (serialPort1.IsOpen)
            {
                stoklar.Clear();
                listBox1.Items.Clear();
                serialPort1.Close();
                listBox1.Items.Add(stoklar.Count);
            }
        }
    }
}
