from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch.conditions import IfCondition

from launch_ros.actions import Node

# cinematica = LaunchConfiguration('cinematica')

# declare_cinematica = DeclareLaunchArgument(
#     'cinematica',
#     default_value='none',
#     description='Tipo de cinematica que vai ser implementada'
# )
# Depois, no Node:
# parameters=[
#     {
#         'radius': radius
#     }
# ]


# launch_volume = LaunchConfiguration('launch_volume')
# declare_launch_volume = DeclareLaunchArgument(
#     'launch_volume',
#     default_value='true',
#     description='Se false, sobe apenas o nó de área.'
# )
# No nó:
# condition=IfCondition(launch_volume)




def generate_launch_description():

    cinematica = LaunchConfiguration('cinematica')

    declare_cinematica = DeclareLaunchArgument(
        'cinematica',
        default_value='none',
        description='Tipo de cinematica que vai ser implementada'
    )


    node_cinematica_ackermann = Node(
        package='cinematica_ackermann',
        executable='cinematica_ackermann_node',
        name='cinematica_ackermann_node',
        # parameters=[caminho_arquivo]
        output='screen',
    )

    node_cinematica_diferencial = Node(
        package='cinematica_diferencial',
        executable='cinematica_diferencial_node',
        name='cinematica_diferencial_node',
        # parameters=[caminho_arquivo]
        output='screen',
    )


    if cinematica == "ackermann":
        return LaunchDescription([
            node_cinematica_ackermann
        ])
    elif cinematica == "diferencial":
        return LaunchDescription([
            node_cinematica_diferencial
        ])
    else:
        print("Opções válidas: diferencial ou ackermann")
        return
