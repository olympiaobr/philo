#!/bin/bash

# Function to run the test and check the results
run_test() {
    local num_philos=$1
    local time_to_die=$2
    local time_to_eat=$3
    local time_to_sleep=$4
    local times_to_eat=$5

    ./philo $num_philos $time_to_die $time_to_eat $time_to_sleep $times_to_eat > output.txt

    # Extract and analyze the timestamps of the death messages
    death_times=$(grep "died" output.txt | awk '{print $1}')

    # Convert timestamps to an array
    IFS=' ' read -r -a times <<< "$death_times"

    # Check the time difference between the death messages
    if [ ${#times[@]} -gt 1 ]; then
        for (( i=0; i<${#times[@]}-1; i++ )); do
            diff=$(( ${times[i+1]} - ${times[i]} ))
            if [ $diff -gt 10 ]; then
                echo "Error: Death delayed by more than 10 ms: $diff ms"
            else
                echo "Death detected within acceptable time frame: $diff ms"
            fi
        done
    else
        echo "Only one death detected within acceptable time frame"
    fi

    # Check the eating count for each philosopher
    for (( i=1; i<=$num_philos; i++ )); do
        eat_count=$(grep "$i is eating" output.txt | wc -l)
        echo "Philosopher $i ate $eat_count times"
        if [ $eat_count -lt $times_to_eat ]; then
            echo "Error: Philosopher $i did not eat $times_to_eat times"
        fi
    done

    # Display the output for verification
    cat output.txt
}

# Run the test for 2 philosophers and different conditions
run_test 2 310 200 100 10
run_test 5 800 200 200 10
run_test 5 800 200 200 7
