#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <string>
#include <regex>

using namespace std;

bool encodeState = false;   //true = encode, false = decode

string charset_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz.1234567890";

string cpt_encode(string input, string key){
    string output = "";
    string tmpkey = key + input;
    int charcode = 0;
    int keycode = 0;
    const char *charset = charset_string.c_str();
    const char *tmp = input.c_str();
    const char *tmpkeyset = tmpkey.c_str();
    int inputlen = input.length();
    int charsetlen = charset_string.length();
    int crypcode = 0;

    for(int i = 0;i<inputlen;i++){
        for(int j = 0;j<charsetlen;j++){
            if(tmp[i] == charset[j]){
                charcode = j;
                break;
            }
        }
        for(int j = 0;j<charsetlen;j++){
            if(tmpkeyset[i] == charset[j]){
                keycode = j;
                break;
            }
        }
        crypcode = (charcode + keycode) % charsetlen;
        output = output + charset[crypcode];
    }
    return output;
}

string cpt_decode(string input, string key){
    string output = "";

    int charcode = 0;
    int keycode = 0;
    const char *charset = charset_string.c_str();
    const char *tmp = input.c_str();
    int inputlen = input.length();
    int charsetlen = charset_string.length();
    int crypcode = 0;

    for(int i = 0;i<inputlen;i++){
        const char *tmpkeyset = key.c_str();
        for(int j = 0;j<charsetlen;j++){
            if(tmp[i] == charset[j]){
                charcode = j;
                break;
            }
        }
        for(int j = 0;j<charsetlen;j++){
            if(tmpkeyset[i] == charset[j]){
                keycode = j;
                break;
            }
        }
        crypcode = (charcode - keycode) % charsetlen;
        if(crypcode<0){
            crypcode = crypcode + charsetlen;
        }
        key = key + charset[crypcode];
        output = output + charset[crypcode];
    }
    return output;
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_radioButton_Encode_clicked(bool checked)
{
    if(checked){
        encodeState = true;
    }
}

void MainWindow::on_radioButton_Decode_clicked(bool checked)
{
    if(checked){
        encodeState = false;
    }
}

void MainWindow::on_pushButton_submit_clicked()
{
    string input = ui->textEdit->toPlainText().toStdString();   //?????????????????????
    string output = ""; //insert output var
    string intmp = "";
    string keytmp = "";
    string key = ui->lineEdit_Key->text().toStdString();

    //transform(input.begin(),input.end(),input.begin(),::toupper);   //???????????????
    //transform(key.begin(),key.end(),key.begin(),::toupper);   //???????????????

    //???????????????????????????
    string pattern("[A-Z]+|[a-z]+|\\.|[0-9]+");
    pattern = "[[:alpha:]]*" + pattern + "[[:alpha:]]*";
    regex r(pattern);
    smatch results;
    //??????????????????
    for (sregex_iterator it(input.begin(),input.end(),r), end_it;it != end_it;++it) {
            intmp = intmp + it->str();
    }
    for (sregex_iterator it(key.begin(),key.end(),r), end_it;it != end_it;++it) {
            keytmp = keytmp + it->str();
    }

    //????????????????????????
    if(encodeState){
        output = cpt_encode(intmp,keytmp);
    }else {
        output = cpt_decode(intmp,keytmp);
    }
    ui->textEdit->setText(output.c_str());
}

