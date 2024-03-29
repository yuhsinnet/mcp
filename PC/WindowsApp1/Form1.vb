﻿Imports EasyModbus
Imports System.Text

Public Class Form1

    Dim cc As ModbusClient

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load

        TextBox2.ImeMode = ImeMode.Off
        TextBox3.ImeMode = ImeMode.Off


        For Each port As String In My.Computer.Ports.SerialPortNames

            ComboBox1.Items.Add(port)

        Next
        ComboBox1.SelectedIndex = 0

        cc = New ModbusClient(ComboBox1.SelectedItem.ToString())
        'cc.LogFileFilename = "C:\Users\SG\Source\gitProject\mcp\PC\WindowsApp1\modbus_log.txt"
        cc.Disconnect()
        cc.Baudrate = 38400
        cc.Parity = System.IO.Ports.Parity.None
        cc.StopBits = System.IO.Ports.StopBits.One

        Button1.Enabled = False


    End Sub

    Private Sub Connect_But_Click(sender As Object, e As EventArgs) Handles Connect_But.Click


        If cc.Connected = False Then

            cc.SerialPort = ComboBox1.SelectedItem.ToString()
            cc.Connect()


            TextBox1.Text = cc.ReadInputRegisters(0, 2)(0).ToString()
            Dim HoReg() = cc.ReadHoldingRegisters(0, 2)

            TextBox2.Text = HoReg(0).ToString()
            TextBox3.Text = HoReg(1).ToString()



            Timer1.Enabled = True

            Connect_But.Text = "Close"
            Button1.Enabled = True


        Else

            Timer1.Enabled = False
            System.Threading.Thread.Sleep(250)

            cc.Disconnect()

            Button1.Enabled = False
            Connect_But.Text = "Connect"
        End If



    End Sub


    Private Sub Timer1_Tick(sender As Object, e As EventArgs) Handles Timer1.Tick
        Timer1.Enabled = False
        TextBox1.Text = cc.ReadInputRegisters(0, 1)(0).ToString()
        'TextBox4.Text = Encoding.ASCII.GetString(cc.sendData)

        Dim data As String

        For Each bytes As Byte In cc.receiveData

            data = data + Hex(bytes).ToString.PadLeft(2, "0") + " "

        Next

        TextBox5.Text = data

        Timer1.Enabled = True



    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        Button1.BackColor = Color.FromKnownColor(KnownColor.LightPink)
        Dim Reg() As Integer = {CInt(TextBox2.Text), CInt(TextBox3.Text)}

        cc.WriteMultipleRegisters(0, Reg)

        Timer2.Enabled = True
    End Sub

    Private Sub Timer2_Tick(sender As Object, e As EventArgs) Handles Timer2.Tick

        Button1.BackColor = Color.FromKnownColor(KnownColor.Control)

        Timer2.Enabled = False

    End Sub

    Private Sub Button2_Click(sender As Object, e As EventArgs) Handles Button2.Click

        Dim Present_value As Integer = CInt(TextBox1.Text)
        Dim precent As Single = CType(TextBox4.Text, Single) * 0.01
        Dim diff_value As Integer = Present_value * precent

        TextBox3.Text = Present_value + diff_value
        TextBox2.Text = Present_value - diff_value
    End Sub

    Private Sub Label2_Click(sender As Object, e As EventArgs) Handles Label2.Click

        Dim Present_value As Integer = CInt(TextBox1.Text)
        Dim precent As Single = CType(TextBox4.Text, Single) * 0.01
        Dim diff_value As Integer = Present_value * precent

        TextBox2.Text = Present_value + diff_value

    End Sub

    Private Sub Label1_Click(sender As Object, e As EventArgs) Handles Label1.Click
        Dim Present_value As Integer = CInt(TextBox1.Text)
        Dim precent As Single = CType(TextBox4.Text, Single) * 0.01
        Dim diff_value As Integer = Present_value * precent

        TextBox3.Text = Present_value + diff_value
    End Sub
End Class
