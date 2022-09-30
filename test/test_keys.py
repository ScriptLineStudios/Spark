import spark
import unittest

spark.init("Key Test", 1, 1, False)

class TestTexure(unittest.TestCase):
    def test_key_pressed(self):
        self.assertFalse(spark.key.key_is_pressed(65))
        self.assertFalse(spark.key.key_is_pressed(66))
        self.assertFalse(spark.key.key_is_pressed(67))
        self.assertFalse(spark.key.key_is_pressed(68))
        self.assertFalse(spark.key.key_is_pressed(69))
        self.assertFalse(spark.key.key_is_pressed(70))
        self.assertFalse(spark.key.key_is_pressed(71))

        self.assertFalse(spark.key.key_is_pressed(spark.key.A))
        self.assertFalse(spark.key.key_is_pressed(spark.key.B))
        self.assertFalse(spark.key.key_is_pressed(spark.key.C))
        self.assertFalse(spark.key.key_is_pressed(spark.key.D))
        self.assertFalse(spark.key.key_is_pressed(spark.key.E))
        self.assertFalse(spark.key.key_is_pressed(spark.key.F))
        self.assertFalse(spark.key.key_is_pressed(spark.key.G))

    def test_keys(self):
        self.assertEqual(spark.key.A, 65)
        self.assertEqual(spark.key.B, 66)
        self.assertEqual(spark.key.C, 67)
        self.assertEqual(spark.key.D, 68)
        self.assertEqual(spark.key.E, 69)
        self.assertEqual(spark.key.SPACE, 32)

if __name__ == "__main__":
    unittest.main()
