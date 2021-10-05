table 57036 "SIT CommisionAttribute"
{
    Caption = 'Commision Attribute';
    DataClassification = CustomerContent;
    Description = 'SC210519';
    DrillDownPageID = "SIT CommisionAttributes";
    LookupPageID = "SIT CommisionAttributes";

    fields
    {
        field(1; "Code"; Code[20])
        {
            Caption = 'Code';
            DataClassification = CustomerContent;
            Description = 'SC210519';
            NotBlank = true;
        }
        field(2; Description; Text[50])
        {
            Caption = 'Description';
            DataClassification = CustomerContent;
            Description = 'SC210519';
        }
        field(3; "Attribute Values"; Integer)
        {
            CalcFormula = Count("SIT CommisionAttributeValue" where("Attribute Code" = field(Code)));
            Caption = 'No. of Attribute Values';
            Description = 'SC210519';
            Editable = false;
            FieldClass = FlowField;
        }
    }

    keys
    {
        key(PK; "Code")
        {
            Clustered = true;
        }
    }

    trigger OnDelete()
    begin
        CalcFields("Attribute Values");
        if ("Attribute Values" <> 0) then
            Error(CanNotBeDeletedErr, TableCaption, FieldCaption("Attribute Values"));
    end;

    var
        CanNotBeDeletedErr: Label '%1 can not be deleted as long as %2 exists.';
}

