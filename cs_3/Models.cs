using System.Collections.Generic;

namespace ValidationLab;

public sealed class Address
{
    [Required]
    [StringLength(2, 80)]
    public string? City { get; set; }

    [Required]
    [Email]
    public string? Email { get; set; }
}

public sealed class Order
{
    [Required]
    [StringLength(3, 100)]
    public string? Name { get; set; }

    [Range(0.01, 100000)]
    public double Sum { get; set; }
}

public sealed class Customer
{
    [Required]
    [StringLength(2, 50)]
    public string? Name { get; set; }

    [ValidateObject]
    public Address? Address { get; set; }

    [ValidateObject]
    public List<Order> Orders { get; set; } = new();
}
