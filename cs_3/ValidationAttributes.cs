using System;
using System.Text.RegularExpressions;

namespace ValidationLab;

[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
public sealed class RequiredAttribute : Attribute
{
}

[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
public sealed class StringLengthAttribute : Attribute
{
    public int MinLength { get; }
    public int MaxLength { get; }

    public StringLengthAttribute(int minLength, int maxLength)
    {
        if (minLength < 0 || maxLength < minLength)
            throw new ArgumentOutOfRangeException(nameof(minLength));

        MinLength = minLength;
        MaxLength = maxLength;
    }
}

[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
public sealed class RangeAttribute : Attribute
{
    public double Min { get; }
    public double Max { get; }

    public RangeAttribute(double min, double max)
    {
        if (double.IsNaN(min) || double.IsNaN(max) ||
            double.IsInfinity(min) || double.IsInfinity(max) || min > max)
            throw new ArgumentOutOfRangeException(nameof(min));

        Min = min;
        Max = max;
    }
}

[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
public sealed class EmailAttribute : Attribute
{
    public bool IsValid(string value)
    {
        return value.Length <= 254 &&
               Regex.IsMatch(value,
                   @"^[A-Za-z0-9.!#$%&'*+/=?^_`{|}~-]+@[A-Za-z0-9-]+(?:\.[A-Za-z0-9-]+)+$");
    }
}

[AttributeUsage(AttributeTargets.Field | AttributeTargets.Property)]
public sealed class ValidateObjectAttribute : Attribute
{
}
