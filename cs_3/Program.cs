using System;
using System.Collections.Generic;

namespace ValidationLab;

internal static class Program
{
    private static void Main()
    {
        while (true)
        {
            try
            {
                Customer customer = new()
                {
                    Name = "A",
                    Address = new Address
                    {
                        City = "T",
                        Email = "invalid-email"
                    },
                    Orders = new List<Order>
                    {
                        new() { Name = "OK", Sum = 100 },
                        new() { Name = null, Sum = -10 }
                    }
                };

                IReadOnlyList<ValidationError> errors = Validator.Validate(customer);

                if (errors.Count == 0)
                {
                    Console.WriteLine("Validation successful.");
                }
                else
                {
                    Console.WriteLine($"Found {errors.Count} validation error(s):");

                    foreach (ValidationError error in errors)
                        Console.WriteLine($"- {error}");
                }
            }
            catch (Exception exception)
            {
                Console.WriteLine($"Error: {exception.Message}");
                Console.WriteLine("1. Continue");
                Console.WriteLine("2. Ignore and continue");
                Console.WriteLine("3. Exit");

                string? choice = Console.ReadLine();

                if (choice == "3")
                    return;

                if (choice != "1" && choice != "2")
                {
                    Console.WriteLine("Invalid choice. Exiting.");
                    return;
                }
            }

            Console.WriteLine("\n1. Run validation again");
            Console.WriteLine("2. Exit");

            if (Console.ReadLine() != "1")
                return;
        }
    }
}
