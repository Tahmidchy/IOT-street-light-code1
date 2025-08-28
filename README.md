# IOT-street-light-code1
street light project source code 1
This is a portion of my research for my academic final-year project, through which I have worked to modernize street lights in order to save power and tax money by Up to 60%. I have also suggested ways to link more cutting-edge technology and real-time fault detection. With this, I hope to build the smart city of my dreams.

Great news! Alhamdulillah Our research article was approved for the IEEE 3rd ECCE International Conference, with the IEEE conference version publication process currently underway. It's worth noting that approximately 40% of the world's energy is being wasted due to the poor management of street lights, resulting in a market value of $20 billion - equivalent to the GDP of some entire nations.

 YOUTUBE LINK Project Demo- https://youtu.be/PgAbA9QPVho
 YOUTUBE LINK Mismanagement of street lights Our investigation -https://youtu.be/3Hq_t2ARukI
 YOUTUBE LINK 3rd ECCE 2023 IEEE Conference CUET Presentation Paper ID-129 - https://youtu.be/ce1P6Of5jzY
 
# Street Lights IOT
SmartLights IoT is a system to manage and optimize the street lighting system. More details on operation and specifications can be found in the [documentation](docs/documentation.pdf).

To provide a small overview, the 3 main modes are the following.

### Day mode
![assets/day.gif](assets/day.gif)

### Night mode
![assets/night.gif](assets/night.gif)

### Night mode eco-friendly
![assets/night-eco.gif](assets/night-eco.gif)

## Quickstart

1. Create a `.env` file with environmental variables

        POSTGRES_PASSWORD=password   
        POSTGRES_USER=street-lights-iot   
        POSTGRES_DB=resources   

        DOCKER_INFLUXDB_INIT_MODE=setup   
        DOCKER_INFLUXDB_INIT_USERNAME=street-lights-iot  
        DOCKER_INFLUXDB_INIT_PASSWORD=password  
        DOCKER_INFLUXDB_INIT_ORG=street-lights-iot
   <img width="1238" height="718" alt="image" src="https://github.com/user-attachments/assets/37edb827-fb72-431f-b3db-5b563f85179e" />

