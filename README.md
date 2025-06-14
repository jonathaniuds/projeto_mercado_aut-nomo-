# 🛒 Carrinho de Compras Inteligente com RFID

Este projeto é um protótipo de **mercado autônomo** usando Arduino. O sistema simula um carrinho inteligente, onde os produtos são identificados por **etiquetas RFID** e o total da compra é exibido em um **display OLED**.

## 🔧 Componentes utilizados

- Arduino (UNO ou Mega)
- Leitor RFID MFRC522
- Display OLED SH1106 (I2C)
- Módulo Bluetooth (HC-05 ou similar)
- LED verde e vermelho
- Buzzer
- Botão físico
- Tags RFID

## 🚀 Funcionalidades

- Leitura de produtos via RFID
- Soma automática dos preços
- Exibição no display OLED (produto, preço e quantidade)
- Finalização de compra com botão
- Envio da nota via Bluetooth

## 🧾 Produtos cadastrados

- Leite – R$ 4,50  
- Macarrão – R$ 2,30  
- Arroz – R$ 20,00  

## 📝 Como funciona

1. Aproxime uma tag RFID do leitor.
2. O sistema reconhece o produto e soma o valor.
3. O display mostra o nome e o total da compra.
4. Pressione o botão para finalizar e enviar os dados via Bluetooth.

## 👨‍💻 Autor

Desenvolvido por **Jonathan Iuds**  
Estudante de Análise e Desenvolvimento de Sistemas  
[GitHub](https://github.com/jonathaniuds)
[Linkedin](https://www.linkedin.com/in/jonathan-nogarolli/)
