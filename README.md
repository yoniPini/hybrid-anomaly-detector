# Anomaly Detection Server

This is the README file for the Anomaly Detection Server, a C++ program that provides anomaly detection functionalities using a hybrid algorithm. The server runs on a separate thread and allows users to interact with it through a menu-based interface.

## Features

The Anomaly Detection Server offers the following features:

1. **Server:** The server runs on a separate thread, allowing it to handle multiple client connections concurrently.

2. **Hybrid Anomaly Detection Algorithm:** The server employs a hybrid anomaly detection algorithm. It first checks the linear correlation between two features in a time series dataset. If the gap between the correlated features exceeds a certain threshold, the algorithm utilizes the Min Circle algorithm to detect irregularities.

3. **Menu-based Interface:** The server provides a menu-based interface for users to interact with. Users can choose from various options to upload datasets, configure algorithm settings, detect anomalies, display results, upload anomalies for analysis, or exit the program.

## Menu Options

The menu options available to the user are as follows:

```
Welcome to the Anomaly Detection Server.
Please choose an option:
1. Upload a time series CSV file.
2. Algorithm settings.
3. Detect anomalies.
4. Display results.
5. Upload anomalies and analyze results.
6. Exit.
```

For each option selected by the user, the server responds with the appropriate action or information.

## Dependencies

The Anomaly Detection Server has the following dependencies:

- [C++ Compiler]: A compatible C++ compiler is required to build and run the program.

- [Threading Library]: The server utilizes threading capabilities, so a suitable threading library is necessary.

- [CSV Parsing Library]: The server requires a library to parse CSV files and extract time series data.

Ensure that the necessary dependencies are installed and accessible to successfully compile and run the Anomaly Detection Server.

## Further Development

The Anomaly Detection Server can serve as a foundation for further development and enhancements. Consider the following areas for potential improvements:

- Expand the anomaly detection algorithm to support additional techniques or algorithms for improved accuracy.

- Implement advanced data visualization features to enhance result display.

- Enhance error handling and error reporting for better user experience.

- Improve performance by optimizing the server's threading capabilities.

- Add support for additional file formats and data sources.

## Conclusion

The Anomaly Detection Server is a C++ program that provides anomaly detection capabilities using a hybrid algorithm. The server runs on a separate thread and offers a menu-based interface for user interaction. The server's features include the hybrid algorithm, the Min Circle algorithm, and various options for uploading datasets, configuring algorithm settings, detecting anomalies, displaying results, and uploading anomalies for analysis.

For more details on how to use and extend the Anomaly Detection Server, please refer to the program's source code and accompanying documentation.

