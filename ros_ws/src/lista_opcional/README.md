# Lista Opcional

Esta pasta contém a implementação dos exercícios da lista opcional de ROS 2, utilizando mensagens personalizadas e nós em Python e C++.

## Estrutura

### `robot_interfaces`

Pacote responsável pelas mensagens personalizadas utilizadas nos exercícios.

Mensagens implementadas:

* `LaserData.msg`
* `Path.msg`
* `RobotPose.msg`
* `RobotState.msg`
* `SensorInfo.msg`
* `VelocityHistory.msg`

### `robot_nodes`

Pacote responsável pelos publishers e subscribers.

#### Publishers em Python

Localizados em `scripts/`:

* `laser_data_publisher.py`
* `path_publisher.py`
* `robot_pose_publisher.py`
* `robot_state_publisher.py`
* `sensor_info_publisher.py`
* `velocity_history_publisher.py`

#### Subscribers em C++

Localizados em `src/`:

* `laser_data_subscriber.cpp`
* `path_subscriber.cpp`
* `robot_pose_subscriber.cpp`
* `velocity_history_subscriber.cpp`

## Exercícios

A lista aborda a criação e utilização de mensagens personalizadas para representar diferentes informações de um robô:

* **RobotState** — estado do robô, incluindo velocidades, bateria e habilitação.
* **VelocityHistory** — histórico de velocidades lineares e angulares.
* **LaserData** — dados de medições de um sensor laser.
* **RobotPose** — pose e velocidade do robô.
* **Path** — trajetória composta por várias poses.
* **SensorInfo** — informações de um sensor.

## Compilação

Dentro do workspace:

```bash
colcon build --symlink-install
source install/setup.bash
```

## Execução

Algumas questões possuem apenas um **publisher**, enquanto outras possuem **publisher e subscriber**.

Nas questões que possuem os dois, é necessário executar o **publisher e o subscriber correspondentes à mesma questão**, ou seja, com o mesmo nome. O publisher é responsável por publicar a mensagem e o subscriber por recebê-la e processá-la.

Exemplo:

```bash
ros2 run robot_nodes path_publisher
ros2 run robot_nodes path_subscriber
```

Nas questões que possuem apenas publisher, basta executar o publisher correspondente.
