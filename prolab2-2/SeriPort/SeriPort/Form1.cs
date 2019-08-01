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
using System.Threading;
using System.IO.Ports;

namespace SeriPort
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            timer1.Interval = 1000;
            timer1.Enabled = true;
        }
        public string[] bilgiler;
        public string cikolata, cay, biskuvi, kahve, su, krs25, krs50, tl1;
        int randomSayi = 0;

        Random rnd = new Random();

        private void timer1_Tick(object sender, EventArgs e)
        {/*
            randomSayi = rnd.Next(0, 5);
            label3.Text = randomSayi.ToString();

            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = "COM3";
                serialPort1.BaudRate = 115200;
                serialPort1.StopBits = StopBits.One;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.Handshake = Handshake.None;

                try
                {
                    serialPort1.Open();
                    serialPort1.Write("00"+randomSayi.ToString()+"R");
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                try
                {
                    serialPort1.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

            */
        }

        private void bilgileriAyir(String yazi)
        {
            int indeks;
            string urun;

            if (yazi.Contains("su") || yazi.Contains("cay") || yazi.Contains("kahve") ||
                yazi.Contains("biskuvi") || yazi.Contains("cikolata"))
            {
                indeks = yazi.IndexOf(",");
                yazi = yazi.Substring(indeks + 1); // birinci virgülü ele
                indeks = yazi.IndexOf(",");
                urun = yazi.Substring(0, indeks);
                yazi = yazi.Substring(indeks + 1); // ikinci virgülü ele
                indeks = yazi.IndexOf(",");
                yazi = yazi.Substring(0, indeks); // üçüncü virgüle kadar olan stringi al
                                                    // yani stok sayısı
                listBox2.Items.Add(urun + " : " + yazi);
                

                if(urun.Equals("su"))
                {
                    for (int i = 0; i < (3- yazi.Length); i++)
                        su += "0";
                    su += yazi + "S";
                }
                else if (urun.Equals("cay"))
                {
                    for (int i = 0; i < (3 - yazi.Length); i++)
                        cay += "0";
                    cay += yazi + "C";
                }
                else if (urun.Equals("kahve"))
                {
                    for (int i = 0; i < (3 - yazi.Length); i++)
                        kahve += "0";
                    kahve += yazi + "K";
                }

                else if (urun.Equals("cikolata"))
                {
                    for (int i = 0; i < (3 - yazi.Length); i++)
                        cikolata += "0";
                    cikolata += yazi + "c";
                }

                else if (urun.Equals("biskuvi"))
                {
                    for (int i = 0; i < (3 - yazi.Length); i++)
                        biskuvi += "0";
                    biskuvi += yazi + "B";
                }

            }
            else
            {
                string temp;

                indeks = yazi.IndexOf(",");
                temp = yazi.Substring(0, indeks);
                for (int i = 0; i < (3 - temp.Length); i++)
                    krs25 += "0";
                krs25 += temp + "Y";


                yazi = yazi.Substring(indeks + 1);

                indeks = yazi.IndexOf(",");
                temp = yazi.Substring(0, indeks);
                for (int i = 0; i < (3 - temp.Length); i++)
                    krs50 += "0";
                krs50 += temp + "E";

                yazi = yazi.Substring(indeks + 1);

                temp = yazi.Substring(0, yazi.Length);

                for (int i = 0; i < (3 - temp.Length); i++)
                    tl1 += "0";
                tl1 += temp + "T";

            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            listBox1.Items.Clear();
            listBox2.Items.Clear();
            string dosya_yolu = @"C:\\Users\\Hacı Bayram ÜNAL\\Desktop\\urunler.txt";
            FileStream fs = new FileStream(dosya_yolu, FileMode.Open, FileAccess.Read);
            StreamReader sw = new StreamReader(fs);
            string yazi = sw.ReadLine();
            while (yazi != null)
            {
                bilgileriAyir(yazi);
                listBox1.Items.Add(yazi);
                yazi = sw.ReadLine();
            }
            sw.Close();
            fs.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            
            if(!serialPort1.IsOpen)
            {
                serialPort1.PortName = "COM3";
                serialPort1.BaudRate = 115200;
                serialPort1.StopBits = StopBits.One;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.Handshake = Handshake.None;

                listBox2.Items.Clear();

                listBox2.Items.Add(su);
                listBox2.Items.Add(cay);
                listBox2.Items.Add(kahve);
                listBox2.Items.Add(biskuvi);
                listBox2.Items.Add(cikolata);
                listBox2.Items.Add(krs25);
                listBox2.Items.Add(krs50);
                listBox2.Items.Add(tl1);

                try
                { 
                    serialPort1.Open();
                    serialPort1.Write(su.Trim());
                    serialPort1.Write(cikolata.Trim());
                    serialPort1.Write(kahve.Trim());
                    serialPort1.Write(biskuvi.Trim());
                    serialPort1.Write(cay.Trim());
                    serialPort1.Write(krs25.Trim());
                    serialPort1.Write(krs50.Trim());
                    serialPort1.Write(tl1.Trim());
                    serialPort1.Write("001R");
                }
                catch(Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }

                try
                {
                    serialPort1.Close();
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

        }
    }
}
