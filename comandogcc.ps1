# Compilação dos arquivos C
C:\MinGW\bin\gcc.exe -g c:\Users\guilh\OneDrive\Documentos\Analex_Anasint\DoBlock.c c:\Users\guilh\OneDrive\Documentos\Analex_Anasint\Analex.c c:\Users\guilh\OneDrive\Documentos\Analex_Anasint\Anasint.c -o c:\Users\guilh\OneDrive\Documentos\Analex_Anasint\output\DoBlock.exe

# Verifica se a compilação foi bem sucedida
if ($?) {
    # Executa o programa compilado
    c:\Users\guilh\OneDrive\Documentos\Analex_Anasint\output\DoBlock.exe
} else {
    Write-Error "Falha na compilação."
}

# Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
