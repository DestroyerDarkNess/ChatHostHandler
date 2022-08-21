Imports System.Runtime.InteropServices
Imports System.Windows.Forms

Public Class DLLMain

    <UnmanagedFunctionPointer(CallingConvention.Cdecl)>
    Public Delegate Sub SAMP_ChatDelegate(ByVal ChatEntered As String)

    <DllImport("ChatHostHandler.asi", CallingConvention:=CallingConvention.Cdecl)>
    Public Shared Sub SetChatCallback(ByVal aCallback As SAMP_ChatDelegate)
    End Sub

    Public Shared Sub DllMain()
        Try

            Dim SAMP_Chat_Thread As SAMP_ChatDelegate = New SAMP_ChatDelegate(AddressOf MainCheat)
            SetChatCallback(SAMP_Chat_Thread)

            Do While True : Loop

        Catch ex As Exception
            MessageBox.Show(ex.Message)
        End Try
    End Sub

    Public Shared Sub MainCheat(ByVal ChatEntered As String)
        If ChatEntered = "/Hello" Then
            MessageBox.Show("Hi bro")
        End If
    End Sub

End Class
