<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form 覆寫 Dispose 以清除元件清單。
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    '為 Windows Form 設計工具的必要項
    Private components As System.ComponentModel.IContainer

    '注意: 以下為 Windows Form 設計工具所需的程序
    '可以使用 Windows Form 設計工具進行修改。
    '請勿使用程式碼編輯器進行修改。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container()
        Me.TextBox1 = New System.Windows.Forms.TextBox()
        Me.Connect_But = New System.Windows.Forms.Button()
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.TextBox2 = New System.Windows.Forms.TextBox()
        Me.TextBox3 = New System.Windows.Forms.TextBox()
        Me.Button1 = New System.Windows.Forms.Button()
        Me.Timer2 = New System.Windows.Forms.Timer(Me.components)
        Me.ComboBox1 = New System.Windows.Forms.ComboBox()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.SuspendLayout()
        '
        'TextBox1
        '
        Me.TextBox1.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.TextBox1.Location = New System.Drawing.Point(188, 111)
        Me.TextBox1.Name = "TextBox1"
        Me.TextBox1.Size = New System.Drawing.Size(119, 59)
        Me.TextBox1.TabIndex = 0
        '
        'Connect_But
        '
        Me.Connect_But.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.Connect_But.Location = New System.Drawing.Point(568, 283)
        Me.Connect_But.Name = "Connect_But"
        Me.Connect_But.Size = New System.Drawing.Size(182, 64)
        Me.Connect_But.TabIndex = 1
        Me.Connect_But.Text = "Connect"
        Me.Connect_But.UseVisualStyleBackColor = True
        '
        'Timer1
        '
        Me.Timer1.Interval = 250
        '
        'TextBox2
        '
        Me.TextBox2.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.TextBox2.Location = New System.Drawing.Point(467, 143)
        Me.TextBox2.Name = "TextBox2"
        Me.TextBox2.Size = New System.Drawing.Size(119, 59)
        Me.TextBox2.TabIndex = 2
        '
        'TextBox3
        '
        Me.TextBox3.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.TextBox3.Location = New System.Drawing.Point(467, 78)
        Me.TextBox3.Name = "TextBox3"
        Me.TextBox3.Size = New System.Drawing.Size(119, 59)
        Me.TextBox3.TabIndex = 3
        '
        'Button1
        '
        Me.Button1.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.Button1.Location = New System.Drawing.Point(599, 82)
        Me.Button1.Name = "Button1"
        Me.Button1.Size = New System.Drawing.Size(151, 107)
        Me.Button1.TabIndex = 4
        Me.Button1.Text = "寫入設定"
        Me.Button1.UseVisualStyleBackColor = True
        '
        'Timer2
        '
        Me.Timer2.Interval = 250
        '
        'ComboBox1
        '
        Me.ComboBox1.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.ComboBox1.FormattingEnabled = True
        Me.ComboBox1.Location = New System.Drawing.Point(568, 226)
        Me.ComboBox1.Name = "ComboBox1"
        Me.ComboBox1.Size = New System.Drawing.Size(182, 51)
        Me.ComboBox1.TabIndex = 5
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.Label1.Location = New System.Drawing.Point(313, 81)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(148, 43)
        Me.Label1.TabIndex = 6
        Me.Label1.Text = "上極限"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.Label2.Location = New System.Drawing.Point(313, 146)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(148, 43)
        Me.Label2.TabIndex = 7
        Me.Label2.Text = "下極限"
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Font = New System.Drawing.Font("新細明體", 32.0!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(136, Byte))
        Me.Label3.Location = New System.Drawing.Point(34, 114)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(148, 43)
        Me.Label3.TabIndex = 8
        Me.Label3.Text = "現在值"
        '
        'Form1
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 12.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(800, 382)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.Label2)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.ComboBox1)
        Me.Controls.Add(Me.Button1)
        Me.Controls.Add(Me.TextBox3)
        Me.Controls.Add(Me.TextBox2)
        Me.Controls.Add(Me.Connect_But)
        Me.Controls.Add(Me.TextBox1)
        Me.Name = "Form1"
        Me.Text = "Form1"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Friend WithEvents TextBox1 As TextBox
    Friend WithEvents Connect_But As Button
    Friend WithEvents Timer1 As Timer
    Friend WithEvents TextBox2 As TextBox
    Friend WithEvents TextBox3 As TextBox
    Friend WithEvents Button1 As Button
    Friend WithEvents Timer2 As Timer
    Friend WithEvents ComboBox1 As ComboBox
    Friend WithEvents Label1 As Label
    Friend WithEvents Label2 As Label
    Friend WithEvents Label3 As Label
End Class
