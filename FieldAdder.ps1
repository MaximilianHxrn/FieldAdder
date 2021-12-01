$folder = $args[0]

# --------------------------------------------------------------

Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing
$form = New-Object System.Windows.Forms.Form
$form.Text = 'FieldAdder'
$form.Size = New-Object System.Drawing.Size(425, 240)
$form.StartPosition = 'CenterScreen'
$form.KeyPreview = $true #This is the important part
$form.Add_KeyDown{
    param ( 
        [Parameter(Mandatory)][Object]$sender,
        [Parameter(Mandatory)][System.Windows.Forms.KeyEventArgs]$e
    )
    if($e.KeyCode -eq "Escape"){
        $form.close()
    }
}

# --------------------------------------------------------------

$labelType = New-Object System.Windows.Forms.Label
$labelType.Text = "Table Type"
$labelType.Location = New-Object System.Drawing.Size(25, 25)
$labelType.Size = New-Object System.Drawing.Size(100, 25)

$DropDown = new-object System.Windows.Forms.ComboBox
$DropDown.DropDownStyle = 'DropDownList'
$DropDown.Location = new-object System.Drawing.Size(125,25)
$DropDown.Size = new-object System.Drawing.Size(250, 20)
[array]$DropDownArray = "table", "tableextension"
ForEach ($Item in $DropDownArray) {
    [void] $DropDown.Items.Add($Item)
}
$DropDown.Text = $args[1]

# --------------------------------------------------------------

$labeltableName = New-Object System.Windows.Forms.Label
$labeltableName.Text = "Table Name"
$labeltableName.Location = New-Object System.Drawing.Size(25, 50)
$labeltableName.Size = New-Object System.Drawing.Size(100, 25)

$tableName = New-Object System.Windows.Forms.TextBox
$tableName.Location = New-Object System.Drawing.Size(125, 50)
$tableName.Size = New-Object System.Drawing.Size(250, 20)
if ($args[2] -eq " ") {
	$tableName.Text = ""
}
else {
	$tableName.Text = $args[2]
}

# --------------------------------------------------------------

$labelFieldID = New-Object System.Windows.Forms.Label
$labelFieldID.Text = "Field ID"
$labelFieldID.Location = New-Object System.Drawing.Size(25, 75)
$labelFieldID.Size = New-Object System.Drawing.Size(100, 25)

$FieldID = New-Object System.Windows.Forms.TextBox
$FieldID.Location = New-Object System.Drawing.Size(125, 75)
$FieldID.Size = New-Object System.Drawing.Size(250, 20)

# --------------------------------------------------------------

$labelFieldName = New-Object System.Windows.Forms.Label
$labelFieldName.Text = "Field Name"
$labelFieldName.Location = New-Object System.Drawing.Size(25, 100)
$labelFieldName.Size = New-Object System.Drawing.Size(100, 25)

$FieldName = New-Object System.Windows.Forms.TextBox
$FieldName.Location = New-Object System.Drawing.Size(125, 100)
$FieldName.Size = New-Object System.Drawing.Size(250, 20)

# --------------------------------------------------------------

$labelFieldType = New-Object System.Windows.Forms.Label
$labelFieldType.Text = "Field Type"
$labelFieldType.Location = New-Object System.Drawing.Size(25, 125)
$labelFieldType.Size = New-Object System.Drawing.Size(100, 25)

$FieldType = New-Object System.Windows.Forms.TextBox
$FieldType.Location = New-Object System.Drawing.Size(125, 125)
$FieldType.Size = New-Object System.Drawing.Size(250, 20)

# --------------------------------------------------------------

$labelRecursion = New-Object System.Windows.Forms.Label
$labelRecursion.Text = "Add more Fields"
$labelRecursion.Location = New-Object System.Drawing.Size(25, 150)
$labelRecursion.Size = New-Object System.Drawing.Size(100, 25)

# --------------------------------------------------------------

$Recursion = New-Object System.Windows.Forms.Checkbox 
$Recursion.Location = New-Object System.Drawing.Size(125, 150) 
$Recursion.Size = New-Object System.Drawing.Size(25, 25)

# --------------------------------------------------------------

$RunButton = New-Object System.Windows.Forms.Button
$RunButton.Location = New-Object System.Drawing.Size(150, 150) 
$RunButton.Size = New-Object System.Drawing.Size(225, 25)
$RunButton.Text = "Add the field to the table"
$RunButton.DialogResult = [System.Windows.Forms.DialogResult]::Yes

# --------------------------------------------------------------

$form.Controls.Add($labelType)
$form.Controls.Add($DropDown)
$form.Controls.Add($labeltableName)
$form.Controls.Add($tableName)
$form.Controls.Add($labelFieldID)
$form.Controls.Add($FieldID)
$form.Controls.Add($labelFieldName)
$form.Controls.Add($FieldName)
$form.Controls.Add($labelFieldType)
$form.Controls.Add($FieldType)
$form.Controls.Add($labelRecursion)
$form.Controls.Add($Recursion)
$form.Controls.Add($RunButton)
$form.Topmost = $true
$form.FormBorderStyle = 'FixedDialog';
$form.MaximizeBox = $false;
$form.MinimizeBox = $false;
$form.KeyPreview = $true #This is the important part
$form.Add_KeyDown{
    param ( 
        [Parameter(Mandatory)][Object]$sender,
        [Parameter(Mandatory)][System.Windows.Forms.KeyEventArgs]$e
    )
    if($e.KeyCode -eq "Escape"){
        $Form.close()
    }
}
$result = $form.ShowDialog()

# --------------------------------------------------------------

if ($result -eq [System.Windows.Forms.DialogResult]::Yes) {
    $branch= &git rev-parse --abbrev-ref HEAD
    $Arguments = @(
        ('"{0}"' -f $DropDown.Text.ToString())
        ('"{0}"' -f $TableName.Text.ToString())
        ('"{0}"' -f $FieldID.Text.ToString())
        ('"{0}"' -f $FieldName.Text.ToString())
        ('"{0}"' -f $FieldType.Text.ToString())
        ('"{0}"' -f $branch)
        ('"{0}"' -f $folder.toString())
    )
    Set-Location "\\sitsrv061\WinFrame\Transfer\cir.al\StandaloneDevTools\FieldAdder"
    .\FieldAdder.exe $Arguments
}
if ($Recursion.Checked -eq $true)
{
    .\FieldAdder.ps1 $folder $DropDown.Text.ToString() $TableName.Text.ToString()
}