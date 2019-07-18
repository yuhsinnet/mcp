Imports EasyModbus


Public Class Form1

    Dim cc As ModbusClient

    Private Sub Form1_Load(sender As Object, e As EventArgs) Handles MyBase.Load







    End Sub

    Private Sub Button1_Click(sender As Object, e As EventArgs) Handles Button1.Click
        cc = New ModbusClient("com3")
        cc.LogFileFilename = "C:\Users\SG\Source\gitProject\mcp\PC\WindowsApp1\modbus_log.txt"
        cc.Disconnect()

        cc.Baudrate = 38400

        cc.Connect()

        TextBox1.Text = cc.ReadInputRegisters(0, 1)(0).ToString()
    End Sub
End Class
