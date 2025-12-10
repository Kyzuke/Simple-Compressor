# 🎛️ Compressor – Audio Plugin (VST3 / AU)

Um compressor moderno, minimalista e colorido, desenvolvido em **JUCE**, com interface inspirada no plugin **Squeezer**.  
Ele oferece uma experiência suave, responsiva e com controle total de dinâmica.

---

## ✨ Recursos
- Interface limpa e moderna  
- Knobs coloridos com identidade visual clara  
- Movimento suave e preciso dos controles  
- Scroll do mouse ajusta valores em passos de **1 unidade**  
- Compatível com:
  - **VST3** (Windows/macOS)
  - **AU** (macOS)

Funciona em todas as DAWs principais:
Reaper, FL Studio, Ableton Live, Logic, Cubase, Studio One, etc.

---

## 🎚️ Parâmetros

| Parâmetro      | Descrição                            | Faixa          |
|----------------|----------------------------------------|----------------|
| **Threshold**  | Nível a partir do qual começa compressão | -60 dB → 0 dB  |
| **Ratio**      | Intensidade da compressão               | 1:1 → 20:1     |
| **Attack**     | Tempo de ataque                        | 0 ms → 200 ms  |
| **Release**    | Tempo de liberação                     | 10 ms → 2000 ms|
| **Makeup Gain**| Compensação de volume                  | 0 dB → +24 dB  |

Cada parâmetro possui uma cor única para facilitar a identificação visual.

---

## 🖥️ Interface (UI)

### 🎨 Design
- Tema escuro  
- Knobs coloridos (Threshold: laranja, Ratio: azul, Attack: verde, Release: roxo, Makeup: amarelo)  
- Curvas suaves e estilização inspirada no Squeezer

### 🖱️ Interação do usuário
- Arraste horizontal ou vertical → ajuste contínuo  
- Scroll do mouse → mudança de **1 unidade por tick**  
- Caixa de texto abaixo de cada knob para digitação direta

---

## 🛠️ Compilação

### Dependências
- **JUCE 7.x**
- C++17  
- CMake ou Projucer

### 🔧 Usando Projucer
1. Abra o arquivo `.jucer`
2. Configure o exportador (Visual Studio / Xcode)
3. Clique em *Save and Open in IDE*
4. Compile em **Release**
