import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.actions import (DeclareLaunchArgument, LogInfo, Shutdown)
from launch.conditions import IfCondition
from launch.substitutions import LaunchConfiguration, PythonExpression

from launch_ros.actions import Node


def generate_launch_description():

    cinematica = LaunchConfiguration('cinematica')

  
    config_ackermann = os.path.join(
        get_package_share_directory('cinematica_ackermann'),
        'config',
        'parametros_ackermann.yaml'
    )

    config_diferencial = os.path.join(
        get_package_share_directory('cinematica_diferencial'),
        'config',
        'parametros_diferencial.yaml'
    )

   
    declare_cinematica = DeclareLaunchArgument(
        name='cinematica',
        default_value='diferencial',
        description='Cinemática a utilizar: diferencial ou ackermann'
    )

  
    expressao_cinematica_invalida = PythonExpression([
        "'",
        cinematica,
        "' not in ['diferencial', 'ackermann']"
    ])


    node_cinematica_ackermann = Node(
        package='cinematica_ackermann',
        executable='cinematica_ackermann_node',
        name='cinematica_ackermann',
        parameters=[config_ackermann],
        output='screen',
        condition=IfCondition(
            PythonExpression([
                "'",
                cinematica,
                "' == 'ackermann'"
            ])
        )
    )

    node_cinematica_diferencial = Node(
        package='cinematica_diferencial',
        executable='cinematica_diferencial_node',
        name='cinematica_diferencial',
        parameters=[config_diferencial],
        output='screen',
        condition=IfCondition(
            PythonExpression([
                "'",
                cinematica,
                "' == 'diferencial'"
            ])
        )
    )


    erro_cinematica = LogInfo(
        msg=(
            "Cinemática inválida! "
            "Use cinematica:=diferencial ou cinematica:=ackermann."
        ),
        condition=IfCondition(expressao_cinematica_invalida)
    )

    encerrar_launch = Shutdown(
        reason='Argumento cinematica inválido.',
        condition=IfCondition(expressao_cinematica_invalida)
    )


    return LaunchDescription([
        declare_cinematica,
        erro_cinematica,
        encerrar_launch,
        node_cinematica_ackermann,
        node_cinematica_diferencial
    ])
