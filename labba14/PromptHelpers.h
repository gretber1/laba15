#pragma once
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

inline String^ Prompt(String^ text, String^ caption)
{
    Form^ f = gcnew Form();
    f->Text = caption;
    f->Size = Size(300, 150);
    f->BackColor = Color::FromArgb(255, 182, 193);
    f->FormBorderStyle = FormBorderStyle::FixedDialog;
    f->StartPosition = FormStartPosition::CenterScreen;

    Label^ lbl = gcnew Label();
    lbl->Text = text;
    lbl->Location = Point(20, 5);
    f->Controls->Add(lbl);

    TextBox^ tb = gcnew TextBox();
    tb->Location = Point(20, 30);
    tb->Width = 240;
    f->Controls->Add(tb);

    Button^ ok = gcnew Button();
    ok->Text = "OK";
    ok->Location = Point(40, 70);
    ok->DialogResult = DialogResult::OK;
    f->Controls->Add(ok);

    Button^ cancel = gcnew Button();
    cancel->Text = "Cancel";
    cancel->Location = Point(140, 70);
    cancel->DialogResult = DialogResult::Cancel;
    f->Controls->Add(cancel);

    f->AcceptButton = ok;
    f->CancelButton = cancel;

    return (f->ShowDialog() == DialogResult::OK) ? tb->Text : "";
}

inline bool PromptDouble(
    String^ text,
    String^ caption,
    double% value,
    double min,
    double max
)
{
    String^ s = Prompt(text, caption);
    if (String::IsNullOrWhiteSpace(s)) return false;

    if (Double::TryParse(s, value))
    {
        if (value < min || value > max)
        {
            MessageBox::Show(
                "Значение должно быть в диапазоне от " +
                min + " до " + max,
                "Ошибка ввода",
                MessageBoxButtons::OK,
                MessageBoxIcon::Error
            );
            return false;
        }
        return true;
    }

    MessageBox::Show(
        "Введено невозможное занчение",
        "Ошибка ввода",
        MessageBoxButtons::OK,
        MessageBoxIcon::Error
    );
    return false;
}
