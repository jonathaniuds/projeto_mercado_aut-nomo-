#include <SPI.h>      //Biblioteca para utilização do protocolo SPI;
#include <MFRC522.h>  //Biblioteca para utilização do circuito RFID MFRC522;

#include <Wire.h>  //Biblioteca para uso do protocolo I2C;
#include <U8g2lib.h>

// Inicialização para Display SH1106 128x64 I2C
U8X8_SH1106_128X64_NONAME_HW_I2C tela(U8X8_PIN_NONE);

#include <SoftwareSerial.h>
SoftwareSerial BT(12, 13);  //13 = RX; 12 = TX; - TX DO MÓDULO VAI NA 13, RX DO MÓDULO VAI NA 12;
#define led_vermelho 6
#define led_verde 7
#define C_SELECT 53  //Pino SDA do módulo;
#define RESET 9      //Pino RESET do módulo MFRC522;

#define BUZZER 5
#define BOTAO 10

MFRC522 rfid(C_SELECT, RESET);  //Declaração do módulo com o nome "rfid" no sistema com os pinos do define;

String dados = "";  //String vazia para armazenar o endereço da tag/cartão RFID;

unsigned long tempo_anterior = 0;
unsigned long intervalo = 2000;

struct Produtos {  // Struct para molde de dados para o vetor (item[])
  String codigo;
  String nome;
  float preco;
};

int itens = 0;
int totalItens = 0;      // Variavel para armazenamento total de itens no carrinho
float precoTotal = 0.0;  // Variavel para armazenamento total dos precos
Produtos item[3];  // Vetor para armazenamento dos itens

void setup() {
  Serial.begin(9600);
  BT.begin(38400);
  SPI.begin();                                //Inicialização do protocolo SPI;
  tela.begin();                               // Inicializa o display
  tela.setPowerSave(0);                       // Liga o display (desliga o modo de economia de energia)
  tela.setFont(u8x8_font_chroma48medium8_r);  // Escolhe uma fonte
  // Você pode atualizar o texto trocando o conteúdo ou limpando a tela

  rfid.PCD_Init();  //Inicialização do módulo RFID;
  Serial.println("RFID: Operacional");
  pinMode(led_vermelho, OUTPUT);  // Setando os compunentes entrada ou saida
  pinMode(led_verde, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BOTAO, INPUT_PULLUP);

  //============== DECLARACAO DE PRODUTOS ===========================
  item[0].codigo = {"66 A0 5E AC", "Leite", 4.50};  // cadastro item 1
  item[1].codigo = {"7A B9 2A D9", "Macarrao", 2.30};  // cadastro item 2
  item[2].codigo = {"0B 00 AD 0D", "Arroz", 20.00};  // cadastro item 3
}

void loop() {
  unsigned long tempo_atual = millis();  //Variável para armazenar o valor da função millis();
  digitalWrite(led_vermelho, LOW);
  digitalWrite(led_verde, LOW);
  delay(500);

  bool estadoAtualBotao = digitalRead(BOTAO);  // Variavel para receber os dados de estado do botao

  if (estadoAtualBotao == LOW) {  // Condicional para enviar a nota fiscal caso o botao seja pressionado
    Serial.print("Disparado!");
    delay(100);
    BT.println(F("  RESUMO DA COMPRA"));
    BT.println(F("------------------\n"));
    BT.print(F("Total itens: "));
    BT.println(totalItens);
    BT.print(F("Preco a pagar: "));
    BT.println(precoTotal);
    BT.println(F("------------------\n"));
    delay(200);
  }

  bool correto = false;  // Variavel para teste de (tags == codigo produto) ? correto = true : correto = false

  if (tempo_atual - tempo_anterior >= intervalo) {  //Temporização que faz com que o RFID realize uma leitura a cada 2 seg;
    tempo_anterior = tempo_atual;                   //Variável tempo_anterior sendo "zerada";

    if (!rfid.PICC_IsNewCardPresent()) {  //If para testar caso o módulo NÃO tenha lido nenhum cartão/tag;
      return;
    }

    if (!rfid.PICC_ReadCardSerial()) {  //If para testar caso o módulo NÃO tenha conseguido ler o endereço do cartão/tag;
      return;
    }
  }

  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    Serial.print("Endereco da TAG (HEX): ");

    for (byte i = 0; i < rfid.uid.size; i++) {  //Loop que percorre o endereço lido no RFID como um vetor;
      if (rfid.uid.uidByte[i] < 0x10) {
        Serial.print(" 0");
      } else {
        Serial.print(" ");
      }

      Serial.print(rfid.uid.uidByte[i], HEX);  //Código para conversão dos dados lidos no módulo, de binário para HEX;

      if (rfid.uid.uidByte[i] < 0x10) {
        dados.concat(String(" 0"));
      } else {
        dados.concat(String(" "));
      }
      dados.concat(String(rfid.uid.uidByte[i], HEX));
    }

    dados.toUpperCase();  //Colocando todos os valores do endereço em caixa alta;
    Serial.println();     //Printa os valores de endereço no Console Serial;

    for (byte i = 0; i < 3; i++) {  // Contador para realizar o teste de igualdade no vetor ITEM

      if (dados.substring(1) == item[i].codigo) {  //Testa se o endereço lido é igual ao contido entre (ITEM[i].codigo) ;
        correto = true;                            // caso seja verdadeiro a variavel (correto) irá receber --> true
        Serial.println("Acesso Permitido!");
        digitalWrite(led_vermelho, LOW);  // Aciona Led_verde caso os dados sejam iguais
        digitalWrite(led_verde, HIGH);

        totalItens++;

        tela.drawString(0, 0, "Produto: ");  // Escreve texto na coluna 0, linha 0
        tela.setCursor(8, 0);
        tela.print(item[i].nome);
        tela.drawString(0, 2, "Preco: ");  // Escreve texto na coluna 0, linha 1
        tela.setCursor(6, 2);
        tela.print(item[i].preco);
        tela.drawString(0, 4, "Qtd carr: ");
        tela.setCursor(9, 4);
        tela.print(totalItens);

        tone(BUZZER, 1000);  // Aciona BUZZER
        delay(2000);
        noTone(BUZZER);

        tela.clear();
        precoTotal = precoTotal + item[i].preco;
      }
    }

    if (correto == true) {
      rfid.PICC_HaltA();
    } else {
      rfid.PICC_HaltA();
      Serial.println("Acesso Negado!");  // Caso seja negativo
      digitalWrite(led_vermelho, HIGH);
      digitalWrite(led_verde, LOW);

      tela.drawString(4, 2, " Produto ");
      tela.drawString(4, 3, "   Nao ");
      tela.drawString(4, 4, "Cadastrado ");
      delay(1000);
      tela.clear();
    }
  }
  dados = "";  //Zera o valor da string para a próxima leitura;
}