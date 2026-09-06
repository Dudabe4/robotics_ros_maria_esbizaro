# Projeto 1 - Publisher e Subscriber

## Subtema

**Controlador de robô utilizando ROS 2**

## Atividades

### 1. Controller Node

**Arquivo:** `controller_node.cpp`

Implementação de um nó ROS 2 em C++ que recebe a posição atual do robô e um objetivo, calcula as velocidades linear e angular para conduzir o robô até o objetivo e publica os comandos no tópico `/cmd_vel`.

O nó:

* recebe o objetivo pelo tópico `/goal`;
* recebe a posição do robô pelo tópico `/robot_position`;
* calcula as velocidades linear e angular;
* publica comandos no tópico `/cmd_vel` a 10 Hz;
* mantém o robô parado enquanto não houver objetivo ou posição do robô recebidos.

## Dependências

* ROS 2
* `rclcpp`
* `geometry_msgs`

## Como executar

Compile o pacote utilizando o sistema de build do ROS 2:

```bash
colcon build
```

Depois, carregue o ambiente:

```bash
source install/setup.bash
```

Execute o nó:

```bash
ros2 run projeto_1 controller_node
```
