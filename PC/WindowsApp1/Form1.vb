Imports EasyModbus


Public Class Form1

    Dim cc As ModbusClient

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load



        cc = New ModbusClient("com3")
        'cc.LogFileFilename = "C:\Users\SG\Source\gitProject\mcp\PC\WindowsApp1\modbus_log.txt"
        cc.Disconnect()
        cc.Baudrate = 38400
        cc.Parity = System.IO.Ports.Parity.None
        cc.StopBits = System.IO.Ports.StopBits.One



    End Sub

    Private Sub Connect_But_Click(sender As Object, e As EventArgs) Handles Connect_But.Click


        If cc.Connected = False Then

            cc.Connect()


            TextBox1.Text = cc.ReadInputRegisters(0, 2)(0).ToString()
            Dim HoReg() = cc.ReadHoldingRegisters(0, 2)

            TextBox2.Text = HoReg(0).ToString()
            TextBox3.Text = HoReg(1).ToString()



            Timer1.Enabled = True

            Connect_But.Text = "Close"

        Else

            Timer1.Enabled = False
            System.Threading.Thread.Sleep(250)

            cc.Disconnect()


            Connect_But.Text = "Connect"
        End If



    End Sub


    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        Timer1.Enabled = False
        TextBox1.Text = cc.ReadInputRegisters(0, 1)(0).ToString()
        Timer1.Enabled = True



    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click

        Dim Reg() As Integer = {CInt(TextBox2.Text), CInt(TextBox3.Text)}

        cc.WriteMultipleRegisters(0, Reg)


    End Sub
End Class
